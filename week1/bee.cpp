#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>

/*
Write classes to represent 3 different types of Bee - Worker, Queen and Drone.
Each Bee has a floating-point health property, which is not writable externally and upon creation is set to a value of 100 (percent).
Each Bee has a Damage() method that takes a single integer parameter that should be a value between 0 and 100. When this method is called, the health of the bee is to be reduced by that percentage of their current health.
When a Worker has a health below 70% it cannot fly and therefore is pronounced Dead. When a Queen has a health below 20%, or a Drone below 50%, it is pronounced dead. This 'Dead' property should be readable from each Bee. When a Bee is dead, no further health deductions should be recorded by the bee, although the Damage() method should still be invokable without error.
Your application should create a single list containing 10 instances of each type of Bee and store in a list or array. It must support methods to allow Damage() to be called for each Bee, and to return the health status of each bee, including whether it is alive or not.
 Your application interface must contains 2 functions ( user press 1 or 2 to activate this function :
 1  Create bee list  Clear current bee list and create new random bees, then display in the console windows
 2  Attack bees   - Attack current bee list , a different random value between 0 and 80 should be selected for each bee and applied with a call to Damage(). After attacked, the user interface should refresh to show the health status of the bees  in console windows
Other requirements:
Application type: Console
*/

enum bee_t {WORKER, QUEEN, DRONE};

class Bee {
  public:
    Bee() {}

    void Damage(int percentage) {
      if (percentage < 0 || percentage > 80)
        return;
      if (!this->_isDead)
        this->health -= this->health * ((float)percentage / 100);
      if (this->health < this->deadThreshold)
        _isDead = true;
    }

    const float& fetchHealth() const {
      return this->health;
    }
    const bool& isDead() const {
      return this->_isDead;
    }
    const char* type() const {
      switch (this->_type) {
        case WORKER:
          return "worker";
        case QUEEN:
          return "queen";
        case DRONE:
          return "drone";
        default:
          return "bee";
      }
    }
    float isAlive() {
      return !this->_isDead;
    }

  protected:
    bool _isDead = false;
    float health = 100;
    float deadThreshold;
    enum bee_t _type;
};

class Worker : public Bee {
  public:
  Worker() {
    deadThreshold = 70;
    _type = WORKER;
  }
};

class Queen : public Bee {
  public:
  Queen() {
    deadThreshold = 20;
    _type = QUEEN;
  }
};

class Drone : public Bee {
  public:
  Drone() {
    deadThreshold = 50;
    _type = DRONE;
  }
};

enum choice {CREATE = 1, ATTACK, EXIT};

const int bee_count = 30;
Bee** bee_list;

void print_bee_list();
void create_bee_list();
void attack_bee_list();

int main(int argc, char** argv) {
  srand(time(0));
  bee_list = new Bee* [bee_count] {};

  int choice = 0;
  int s;
  while (1) {
    print_bee_list();
    printf("1. Create bee list\n");
    printf("2. Attack bee\n");
    printf("3. Exit\n");
    s = scanf("%d", &choice);
    if (s != 1) {
      printf("Input again\n");
      getchar();
      continue;
    }

    switch(choice) {
      case CREATE:
        create_bee_list();
        break;
      case ATTACK:
        attack_bee_list();
        break;
      case EXIT:
        for (int i = 0; i < bee_count; i++) {
          delete bee_list[i];
        }
        delete[] bee_list;
        return 0;
      default:
        break;
    }
  }
  return 0;
}

void print_bee_list() {
  printf("[");
  for (int i = 0; i < bee_count; i++) {
    Bee* bee = bee_list[i];
	if (!bee)
	  continue;
    printf(" %s:%d:%f ", bee->type(), bee->isDead(), bee->fetchHealth());
  }
  printf("]\n");
}

void create_bee_list() {
  for (int i = 0; i < bee_count; i++) {
    delete bee_list[i];
    // bee_t beetype = (bee_t)(rand() % 3);
    bee_t beetype = (bee_t)(i / 10);
    Bee* bee;
    switch (beetype) {
      case WORKER:
        bee = new Worker();
        break;
      case QUEEN:
        bee = new Queen();
        break;
      case DRONE:
        bee = new Drone();
        break;
      default:
        break;
    }
    bee_list[i] = bee;
  }
}

void attack_bee_list() {
  for (int i = 0; i < bee_count; i++) {
    int percent = rand() % 81;
    bee_list[i]->Damage(percent);
  }
}
