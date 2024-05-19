#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 기차열
#define LEN_MIN 15
#define LEN_MAX 50

// 확률
#define PROB_MIN 10
#define PROB_MAX 90

// 어그로
#define AGGRO_MIN 0
#define AGGRO_MAX 5

// 마동석 이동 방향
#define MOVE_LEFT 1
#define MOVE_STAY 0

// 좀비의 공격 대상
#define ATK_NONE 0
#define ATK_CITIZEN 1
#define ATK_DONGSEOK 2

// 마동석 행동
#define ACTION_REST 0
#define ACTION_PROVOKE 1
#define ACTION_PULL 2

#define STM_MIN 0
#define STM_MAX 5

int train_length, madongseok, zombie, citizen, turn = 0, percentile_probability;
int temp_aggro, stamina, move, action, temp_citizen, temp_zombie, temp_aggro, temp_stamina;
int citizen_aggro = 1;
int ma_aggro = 1;
int zombie_captured = 0;  // 좀비가 붙잡혔는지 여부를 나타내는 변수

// 인트로 함수
void print_intro() {
    printf("********************\n");
    printf("**  부산행 게임   **\n");
    printf("********************\n");
    printf("좀비를 피해서 무사히 탈출하세요.\n");
    printf("행운을 빕니다.\n");
    printf("\n");
    printf("게임을 시작합니다\n");
}

// 확률 입력 함수
int input_probability() {
    printf("확률을 입력하세요. (10 ~ 90) >> ");
    scanf("%d", &percentile_probability);
    printf("\n");
    //2-2 입력값 처리
    if (percentile_probability < PROB_MIN || percentile_probability > PROB_MAX) {
        input_probability();
    }

    return percentile_probability;
}

// 마동석 스태미나 함수
int madonseok_stamina() {
    printf("\n");
    printf("마동석 스태미나 (0 ~ 5) >> ");
    scanf("%d", &stamina);
    printf("\n");

    if (stamina < STM_MIN || stamina > STM_MAX) {
        madonseok_stamina();
    }
    return stamina;
}

// 열차의 길이 입력 함수
void input_train_length() {
    printf("\n");
    printf("열차의 길이를 입력해주세요. (15 ~ 50) >> ");
    scanf("%d", &train_length);
    printf("\n");
    //2-2 입력값
    if (train_length < LEN_MIN || train_length > LEN_MAX) {
        input_train_length();
    }

    madongseok = train_length - 2;
    zombie = train_length - 3;
    citizen = train_length - 6;
}

// 열차 상태 출력 함수
void print_train_line(char character, int train_length) {
    for (int i = 0; i < train_length; i++) {
        printf("%c", character);
    }
    printf("\n");
}

void print_train() {
    print_train_line('#', train_length); // 첫 번째 줄 출력
    for (int i = 0; i < train_length; i++) {
        if (i == citizen) {
            printf("C");
        } else if (i == madongseok) {
            printf("M");
        } else if (i == zombie) {
            printf("Z");
        } else if (i == 0 || i == train_length - 1) {
            printf("#");
        } else {
            printf(" ");
        }
    }
    printf("\n");
    print_train_line('#', train_length); // 세 번째 줄 출력
}

// 시민 이동 함수
void move_citizen() {
    temp_citizen = citizen;
    int rcitizen = rand() % 101;
    if (100 - percentile_probability >= rcitizen) {
        citizen -= 1; // 시민을 움직임
    }
}

void print_move_citizen() {
    if (temp_citizen == citizen) {
        temp_aggro = citizen_aggro;
        citizen_aggro -= 1;
        if (citizen_aggro < AGGRO_MIN) {
            citizen_aggro = AGGRO_MIN;
        }
        printf("citizen: stay %d (aggro %d -> %d)\n", citizen, temp_aggro, citizen_aggro);
    } else {
        temp_aggro = citizen_aggro;
        citizen_aggro += 1;
        if (citizen_aggro > AGGRO_MAX) {
            citizen_aggro = AGGRO_MAX;
        }
        printf("citizen: %d -> %d (aggro: %d -> %d)\n", temp_citizen, citizen, temp_aggro, citizen_aggro);
    }
}

// 좀비 이동 함수
void move_zombie() {
    temp_zombie = zombie;
    int rzom = rand() % 101;

    if (zombie_captured > 0) {
        zombie_captured--;  // 좀비가 붙잡혀 있으면 턴을 줄임
        return;  // 좀비가 붙잡혀 있으면 이동하지 않음
    }

    if (turn % 2 != 0) { // 홀수 턴일 때만 좀비가 움직임
        if (100 - percentile_probability <= rzom) {
            zombie -= 1; // 좀비를 움직임
        }else {
            temp_zombie = zombie;
        }
    }
}

void print_move_zombie() {
    if (turn % 2 != 0) {
        if (zombie_captured > 0) {
            printf("zombie: stay %d (captured)\n", zombie);
        } else if (temp_zombie == zombie) {
            printf("zombie: stay %d\n", zombie);
        } else {
            printf("zombie: %d -> %d\n", temp_zombie, zombie);
        }
    } else {
        printf("zombie: stay %d (cannot move)\n", zombie);
    }
    // 좀비의 이동 방향을 결정하는 부분 수정
    if(turn % 2 != 0) {
        if(ma_aggro > citizen_aggro) {
            // 마동석 쪽으로 이동
            if (zombie + 1 < madongseok) {
                zombie += 1;
         }
        } else {
            // 시민 쪽으로 이동
            if (zombie - 1 > citizen) {
                zombie -= 1;
        }
    }
}
}


// 마동석 이동 함수
void ma_move() {
    temp_aggro = ma_aggro;
    int temp_ma = madongseok;
    if(zombie != madongseok - 1) {
    printf("madongseok move (0: stay, 1: left) >> ");
    scanf("%d", &move);
    } else {
        printf("madongseok move(0: stay) >>");
        scanf("%d", &move);
    }
    if (move == MOVE_STAY) {
        temp_ma = madongseok;
    } else {
        madongseok -= 1;
}
}
//마동석 이동 출력 함수
void print_ma_move() {
        temp_aggro = ma_aggro;
        if(move == MOVE_STAY ) {
        ma_aggro -= 1;
        printf("madongseok: stay %d (aggro: %d -> %d, stamina: %d)\n", madongseok, temp_aggro, ma_aggro, stamina);
        } else {
        ma_aggro += 1;
        printf("madongseok: left %d (aggro: %d -> %d, stamina: %d)\n", madongseok, temp_aggro, ma_aggro, stamina);
        }
        if (ma_aggro < AGGRO_MIN) {
            ma_aggro = AGGRO_MIN;
        }
        if (ma_aggro > AGGRO_MAX) {
            ma_aggro = AGGRO_MAX;
        }
}


// 마동석 행동 함수
void ma_action() {
    int mar = rand() % 101;
    int temp_stamina = stamina;
    temp_aggro = ma_aggro;
    if(madongseok == zombie + 1) {
        printf("madongseok action (0: rest, 1: provoke, 2: pull >>)");
    } else {
        printf("madongseok action (0: rest, 1: provoke) >> ");
    }
    scanf("%d", &action);
    if (action == ACTION_REST) {
        printf("\n");
        printf("madongseok rests...\n");
        stamina += 1;
        ma_aggro -= 1;
        if (stamina > STM_MAX) stamina = STM_MAX;
        printf("madongseok: %d (aggro: %d -> %d, stamina: %d -> %d)\n", madongseok, temp_aggro, ma_aggro, temp_stamina, stamina);
        if (ma_aggro < AGGRO_MIN) ma_aggro = AGGRO_MIN;
    } else if (action == ACTION_PROVOKE) {
        printf("\n");
        printf("madongseok provoked zombie...\n");
        ma_aggro = AGGRO_MAX;
        printf("madongseok: %d (aggro: %d -> %d, stamina: %d)\n", madongseok, temp_aggro, ma_aggro, stamina);
    } else if (action == ACTION_PULL) {
        printf("\n");
        if (100 - percentile_probability < mar) {
            printf("madongseok pulled zombie... Next turn it can't move.\n");
            ma_aggro += 2;
            stamina -= 1;
            zombie_captured = 1;  // 좀비를 한 턴 동안 붙잡음
            printf("madongseok: %d (aggro: %d -> %d, stamina: %d -> %d)\n", madongseok, temp_aggro, ma_aggro, temp_stamina, stamina);
        } else {
            printf("madongseok failed to pull zombie.\n");
            ma_aggro += 2;
            stamina -= 1;
            printf("madongseok: %d (aggro %d -> %d, stamina: %d -> %d)\n", madongseok, temp_aggro, ma_aggro, temp_stamina, stamina);
        }
    }
}

// 시민 탈출 함수
void happy_ending() {
    printf("시민이 탈출했어요!!\n");
    printf("\n");
    printf("********************\n");
    printf("**   Game Clear!  **\n");
    printf("********************\n"); // 아웃트로
}

// 좀비에게 잡혔을때 나오는 아웃트로 함수
void bad_ending() {
    printf("잡히셨습니다!!\n");
    printf("\n");
    printf("********************\n");
    printf("**   Game Over..  **\n");
    printf("********************\n"); // 아웃트로
}

// 좀비가 시민을 잡았을 때
void zombie_catch() {
    if (citizen == zombie - 1) {
        bad_ending();
        exit(0);
    }
}

// 시민이 탈출했을 때
void citizen_escape() {
    if (citizen == 1) {
        happy_ending();
        exit(0);
    }
}
void result() {
    if(citizen != 1) {
    printf("citizen does nothing.");
    }
    
    if(citizen == zombie - 1 && zombie + 1 == madongseok) {
        temp_stamina = stamina;
        if(ma_aggro >= citizen_aggro) {
            stamina -= 1;
            printf("aggro: %d vs %d, madongseok stamina: %d -> %d", citizen_aggro, ma_aggro, temp_stamina, stamina);
        } else {
            printf("zombie attacked nobody.");
        }
        

    }
}

int main() {
    srand((unsigned int)time(NULL));
    print_intro();
    input_train_length();
    madonseok_stamina();
    input_probability();
    
    // 초기 열차 상태 출력
    print_train();
    printf("\n\n");
    
    while (1) {
        turn++;

        // 시민과 좀비의 움직임 계산
        move_citizen(); // 시민의 움직임 계산
        move_zombie(); // 좀비의 움직임 계산

        // 첫 번째 기차 상태 출력 (시민과 좀비 이동 후)
        print_train(); // 열차 상태 출력
        printf("\n");

        // 시민과 좀비의 이동 상황을 출력
        print_move_citizen(); // 시민의 이동 상황 출력
        print_move_zombie(); // 좀비의 이동 상황 출력
        printf("\n");

        // 마동석의 이동 계산을 먼저 수행
        ma_move();

        // 두 번째 기차 상태 출력 (마동석 이동 후)
        print_train(); // 열차 상태 출력
        printf("\n");

        // 마동석의 이동 상황 출력
        print_ma_move();
        printf("\n");

        // 결과 출력
        result();
        printf("\n");

        // 마동석의 행동 결정
        ma_action();

        // 좀비에게 잡혔는지 확인
        zombie_catch();

        // 시민이 탈출했는지 확인
        citizen_escape();
    }
    
    return 0;
}
