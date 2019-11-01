#include <iostream>
#include <ctime>
#include <vector>
#include <chrono>


void set_random_seed();
int randn(int n);
bool is_banned(const std::vector<int>& banned_vector, int index);

struct solver{
  void feedback(const std::vector<int>& attempt, int& black_hits, int& white_hits){
    //check black hits first
    std::vector<int> banned_from_sequence;
    black_hits = 0;
    white_hits = 0;
    for (int i = 0; i< length; i++){
      if (attempt[i]==temp_sequence[i]){
        black_hits = black_hits + 1;
        banned_from_sequence.push_back(i);
      }
    }
    std::vector<int> banned_from_attempt = banned_from_sequence;//at this point banned from attempt is the same

    for (int j = 0; j < length; j++){
      if (!is_banned(banned_from_sequence, j)){//first we check if j has been banned from black
        //now we want to cycle through attempts
        for (int p = 0; p<length; p++){
          if (!is_banned(banned_from_attempt, p) && !is_banned(banned_from_sequence, j)){
            if (temp_sequence[j]==attempt[p]){
              white_hits = white_hits+1;
              banned_from_attempt.push_back(p);
              banned_from_sequence.push_back(j);

            }
          }
        }
      }
    }

  }

    void start(int i_length, int i_num){
        length = i_length;
        num = i_num;
        run_get_insights_before = false;

        std::vector<int> temp_to_gen_all;
        for (int g = 0; g< length; g++){
          temp_to_gen_all.push_back(0);
        }
        generate_all_guesses(0, temp_to_gen_all);


    }

    void make_attempt(std::vector<int>& attempt){
      attempt = potential_solutions[randn(potential_solutions.size())];
    }

    void get_insights(std::vector<int>& attempt, int black_hits, int white_hits){

      int temp_black_hits = 0;
      int temp_white_hits = 0;
      std::vector<std::vector<int> > next_potential_solutions;

      if (!run_get_insights_before){
        for (int d = 0; d<length;d++){
          temp_sequence.push_back(0);
        }
        run_get_insights_before = true;
      }
      for (int x = 0; x< potential_solutions.size(); x++){
        for (int c = 0; c < potential_solutions[x].size(); c++){
          temp_sequence[c]= potential_solutions[x][c];
          //std::cout<<temp_sequence[c];
        }
        //std::cout<<std::endl;
        feedback(attempt, temp_black_hits, temp_white_hits);
        if (temp_black_hits == black_hits && temp_white_hits == white_hits){
          next_potential_solutions.push_back(temp_sequence);
        }
      }
      potential_solutions = next_potential_solutions;

    }
    void generate_all_guesses(int ind_to_change, std::vector<int> temp){

      for (int i = 0; i< num; i++){
        temp[ind_to_change]=i;
        if (ind_to_change < length-1) {
          generate_all_guesses(ind_to_change+1, temp);
        }
        else if (ind_to_change == length-1){
          potential_solutions.push_back(temp);
        }
      }
    }

    int length;
    int num;
    bool run_get_insights_before;
    std::vector<std::vector<int> > potential_solutions;
    std::vector<int> temp_sequence;
    int no_of_attempts;
};

int main(){

  set_random_seed();

    int total_attempts = 0;
    int length;
    int num;
    std::cout<<"Assign a value to each color you are playing with first."<<std::endl;
    std::cout<<"What is the length of code you are trying to find? ";
    std::cin>>length;
    std::cout<<"What is the total number of values each digit of the code can have? ";
    std::cin>>num;
    std::cout<<std::endl;

    solver thisSolver;
    thisSolver.start(length, num);
    std::vector<int> attempt;

    int black = 0;
    int white = 0;
    int no_of_attempts = 0;

    while (black < length){
      thisSolver.make_attempt(attempt);
      std::cout<<"Attempt this code: ";
      for (int i = 0; i<attempt.size(); i++){
        std::cout<<attempt[i]<<" ";
      }
      std::cout<<"How many blacks did it get? ";
      std::cin>>black;
      std::cout<<std::endl;
      std::cout<<"How many whites did it get? ";
      std::cin>>white;
      std::cout<<std::endl;
      thisSolver.get_insights(attempt,black,white);
      no_of_attempts = no_of_attempts + 1;
    }
    for (int i = 0; i<attempt.size(); i++){
      std::cout<<attempt[i]<<" ";
    }
    std::cout<<std::endl<<"Attempts: "<<no_of_attempts<<std::endl;
    return 0;
}


void set_random_seed(){
    std::srand(std::time(0));
}

int randn(int n){
    return std::rand() % n;
}
bool is_banned(const std::vector<int>& banned_vector, int index){
  for (int i = 0; i< banned_vector.size(); i++){
    if (banned_vector[i]== index){
      return true;
    }
  }
  return false;
}
