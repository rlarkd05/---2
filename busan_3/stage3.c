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

int train_length; //기차
int madongseok; //마동석
int zombie, temp_zombie; //좀비
int citizen, temp_citizen; //시민
int turn = 0; //턴
int percentile_probability; //확률
int temp_aggro; //전 어그
int stamina, temp_stamina; // 스테미나 
int move; //마동석 움직임
int action; //마동석 행동 
int citizen_aggro = 1; //시민 어그로 처음에 1로 지정
int ma_aggro = 1; //마동석 어드로 처음에 1로 지정 
int zombie_captured = 0; //좀비가 붙잡혔는지 여부를 나타내는 변수
int citizens[LEN_MAX]; //시민을 랜덤으로 설정하기 위한 변수
int temp_citizens[LEN_MAX]; //시민들 움직이기 전
int temp_citizens_aggro; //시민들 전 어그로
int citizens_aggro[AGGRO_MAX];//시민들 어그로
int temp_aggros; // 시민들이 이동하기 전
int num_citizens;
int citizens_prob[LEN_MAX]; //시민들의 이동 확률
int new_citizens[LEN_MAX]; // 남은 시민들을 저장할 임시 배열
int can_move = 1; // 이동 가능 여부를 나타내는 변수, 초기값은 이동 가능으로 설정
int citizens_escaped[LEN_MAX];
int total_citizens_escaped = 0;
// 인트로 함수
void print_intro() {
    printf("********************\n");
    printf("**  부산행 게임  **\n");
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
void num_citizen_random() {
    num_citizens = train_length / 4 + rand() % ((train_length - 2) - (train_length - 4) + 1);
    printf("생성된 시민의 수: %d\n", num_citizens);

    citizens[0] = train_length - 6;  // 첫 번째 시민의 위치를 고정
    citizens_prob[0] = rand() % 101;

    for (int i = 0; i < num_citizens; i++) {
        citizens[i] = rand() % (train_length - 8) + 2;
        citizens_prob[i] = rand() % 101;
        for (int j = 0; j < i; j++) {
            if (citizens[i] == citizens[j]) {
                i--;
                break;
            }
        }
    }
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
        if (i == madongseok) {
            printf("M");
        }
        else if (i == zombie) {
            printf("Z");
        }
        else if (i == 0 || i == train_length - 1) {
            printf("#");
        }
        else {
            // 시민의 위치인지 확인하여 출력
            int is_citizen = 0;
            for (int j = 0; j < num_citizens; j++) {
                if (citizens[j] == i) {
                    printf("C");
                    is_citizen = 1;
                    break;
                }
            }
            if (!is_citizen) {
                printf(" ");
            }
        }
    }
    printf("\n");
    print_train_line('#', train_length); // 세 번째 줄 출력
}
void move_citizens() {
    int new_num_citizens = 0;
    int citizen_escaped = 0; // 탈출한 시민이 있는지 확인

    for (int i = 0; i < num_citizens; i++) {
        temp_citizens[i] = citizens[i];
        if (citizens_escaped[i]) continue; // 탈출한 시민은 이동하지 않음

        int rcitizen = rand() % 101;
        if (100 - citizens_prob[i] >= rcitizen) {
            // 시민이 앞에 다른 시민이 없는 경우에만 이동
            if (citizens[i] > 1 && (i == 0 || citizens[i] - 1 != citizens[i - 1])) {
                citizens[i] -= 1;
            }
            if (citizens[i] == 1) {
                citizens_escaped[i] = 1; // 탈출 표시
                citizen_escaped = 1; // 탈출한 시민 있음
                printf("Citizen %d has escaped!\n", i);
            }
        }

        // 시민이 탈출하지 않았으면 새로운 배열에 추가
        if (!citizens_escaped[i]) {
            new_citizens[new_num_citizens] = citizens[i];
            citizens_prob[new_num_citizens] = citizens_prob[i];
            new_num_citizens++;
        }
    }

    // 시민 배열 업데이트
    for (int i = 0; i < new_num_citizens; i++) {
        citizens[i] = new_citizens[i];
        citizens_prob[i] = citizens_prob[i];
    }

    num_citizens = new_num_citizens;
    printf("Remaining citizens: %d\n", num_citizens);

    // 게임 종료 조건 확인
    if (citizen_escaped) {
        printf("Citizen victory! At least one citizen has escaped.\n");
        exit(0);
    }
    if (num_citizens == 0) {
        printf("Bad ending! All citizens have been caught.\n");
        exit(0);
    }
}



void print_move_citizens() {
    for (int i = 0; i < num_citizens; i++) {
        // 시민이 위치 1에 도달하면 -1로 이동시켜 탈출 처리
        if (citizens[i] == 1) {
            citizens[i] = -1;
            continue;
        }

        // 시민이 탈출하지 않았고 기차에 남아있는 경우에만 처리
        if (citizens[i] != -1) {
            int temp_aggro = citizens_aggro[i];
            
            // 시민이 실제로 움직였는지 확인
            if (citizens[i] != temp_citizens[i]) {
                citizens_aggro[i]++;
                if (citizens_aggro[i] > AGGRO_MAX)
                    citizens_aggro[i] = AGGRO_MAX;
                printf("citizen%d: %d -> %d (aggro %d -> %d)\n", i, temp_citizens[i], citizens[i], temp_aggro, citizens_aggro[i]);
            } else {
                printf("citizen%d: stay %d (aggro %d -> %d)\n", i, temp_citizens[i], temp_aggro, citizens_aggro[i]);
            }
        }
    }
}

// 좀비 이동 함수
void move_zombie() {
    temp_zombie = zombie;
    // 좀비가 붙잡혀 있으면 턴을 줄임
    if (zombie_captured > 0) {
        zombie_captured--;
        return; // 좀비가 붙잡혀 있으면 이동하지 않음    
    }
    // 홀수 턴일 때만 좀비가 움직임
    if (turn % 2 != 0) {
        if (!(madongseok == zombie + 1 && zombie == citizen + 1)) {
            if (ma_aggro > citizen_aggro) {
                zombie += 1;
            }
            else if (citizen_aggro > ma_aggro) {
                zombie -= 1;
            }
            else if (citizen_aggro == ma_aggro) {
                zombie -= 1;
            }
            // 좀비가 마동석 쪽으로 이동할 때만 스태미나가 감소
            if (zombie == madongseok - 1) {
                temp_stamina = stamina;
                stamina -= 1;
                printf("stamina: %d -> %d\n", temp_stamina, stamina);
            }
            // 좀비가 마동석과 겹치는지 확인
            if (zombie == madongseok) {
                // 겹치지 않도록 원래 위치로 되돌리기
                zombie = temp_zombie;
            }
        }
        else {
            zombie = temp_zombie;
        }
    }
}

void print_move_zombie() {
    if (turn % 2 != 0) {
        if (temp_zombie == zombie) {
            printf("zombie: stay %d\n", zombie);
        }
        else {
            printf("zombie: %d -> %d\n", temp_zombie, zombie);
        }
    }
    else {
        printf("zombie: stay %d (cannot move)\n", zombie);
    }
}
void check_zombie_catches() {
    for (int i = 0; i < num_citizens; i++) {
        if (citizens[i] == zombie ) {
            printf("Zombie caught citizen %d at position %d!\n", i, citizens[i]);
            citizens_escaped[i] = 1;
            total_citizens_escaped++;
            break;
        }
    }
}

// 마동석 이동 함수
void ma_move() {
    int temp_ma = madongseok;

    if (zombie != madongseok - 1) {
        do {
            printf("madongseok move (0: stay, 1: left) >> ");
            scanf("%d", &move);
        } while (move != MOVE_STAY && move != MOVE_LEFT);
    }
    else {
        do {
            printf("madongseok move (0: stay) >> ");
            scanf("%d", &move);
        } while (move != MOVE_STAY);
    }

    if (move == MOVE_STAY) {
        temp_ma = madongseok;
    }
    else {
        madongseok -= 1;
    }
}


//마동석 이동 출력 함수
void print_ma_move() {
    int temp_aggro = ma_aggro;
    if (move == MOVE_STAY) {
        ma_aggro -= 1;
        if (ma_aggro < AGGRO_MIN) {
            ma_aggro = AGGRO_MIN;
        }
        printf("madongseok: stay %d (aggro: %d -> %d, stamina: %d)\n", madongseok, temp_aggro, ma_aggro, stamina);
    }
    else {
        ma_aggro += 1;
        if (ma_aggro > AGGRO_MAX) {
            ma_aggro = AGGRO_MAX;
        }
        printf("madongseok: left %d (aggro: %d -> %d, stamina: %d)\n", madongseok, temp_aggro, ma_aggro, stamina);
    }
}



// 마동석 행동 함수
void ma_action_rest() {
    if (action == ACTION_REST) {
        int temp_aggro = ma_aggro;
        int temp_stamina = stamina;

        printf("madongseok rests...\n");
        stamina += 1;
        ma_aggro -= 1;
        if (stamina > STM_MAX) stamina = STM_MAX;
        if (ma_aggro < AGGRO_MIN) ma_aggro = AGGRO_MIN;
        printf("madongseok: %d (aggro: %d -> %d, stamina: %d -> %d)\n", madongseok, temp_aggro, ma_aggro, temp_stamina, stamina);
    }
}
//도발할때
void ma_action_provoke() {
    if (action == ACTION_PROVOKE) {
        int temp_aggro = ma_aggro;
        int temp_stamina = stamina;

        printf("madongseok provoked zombie...\n");
        ma_aggro = AGGRO_MAX;
        printf("madongseok: %d (aggro: %d -> %d, stamina: %d)\n", madongseok, temp_aggro, ma_aggro, stamina);
    }
}
//당길때
void ma_action_pull() {
    int temp_aggro = ma_aggro;
    int mar = rand() % 101;
    int temp_stamina = stamina;
    if (action == ACTION_PULL) {
        printf("madongseok attempts to pull zombie...\n");
        if (100 - percentile_probability < mar) {
            printf("madongseok successfully pulled zombie! Zombie is captured for one turn.\n");
            ma_aggro += 2;
            stamina -= 1;
            zombie_captured = 1;  // 좀비를 한 턴 동안 붙잡음
            if (ma_aggro > AGGRO_MAX) ma_aggro = AGGRO_MAX;
            if (stamina < STM_MIN) stamina = STM_MIN;
            printf("madongseok: %d (aggro: %d -> %d, stamina: %d -> %d)\n", madongseok, temp_aggro, ma_aggro, temp_stamina, stamina);
        }
        else {
            printf("madongseok failed to pull zombie.\n");
            ma_aggro += 2;
            stamina -= 1;
            if (ma_aggro > AGGRO_MAX) ma_aggro = AGGRO_MAX;
            if (stamina < STM_MIN) stamina = STM_MIN;
            printf("madongseok: %d (aggro %d -> %d, stamina: %d -> %d)\n", madongseok, temp_aggro, ma_aggro, temp_stamina, stamina);
        }
    }
}
//마동석 행동 입력
void ma_action() {
    int temp_aggro = ma_aggro;

    if (temp_aggro > AGGRO_MAX) {
        temp_aggro = AGGRO_MAX;
    }

    if (madongseok == zombie + 1) {
        do {
            printf("madongseok action (0: rest, 1: provoke, 2: pull) >> ");
            scanf("%d", &action);
        } while (action != ACTION_REST && action != ACTION_PROVOKE && action != ACTION_PULL);
    }
    else {
        do {
            printf("madongseok action (0: rest, 1: provoke) >> ");//좀비가 붙어있지 않으면 pull 안뜸
            scanf("%d", &action);
        } while (action != ACTION_REST && action != ACTION_PROVOKE);
    }
}

// 시민 탈출 함수
void happy_ending() {
    printf("시민이 탈출했어요!!\n");
    printf("\n");
    printf("********************\n");
    printf("**   Game Clear!  **\n");
    printf("********************\n"); // 아웃트로
    exit(1);
}

// 좀비에게 잡혔을때 나오는 아웃트로 함수
void bad_ending() {
    printf("\n");
    printf("********************\n");
    printf("**   Game Over..  **\n");
    printf("********************\n"); // 아웃트로
    exit(1);
}

// 좀비가 시민을 잡았을 때
void zombie_catch_citizens() {
    for (int i = 0; i < num_citizens; i++) {
        if (citizens[i] == zombie - 1) {
            if (madongseok == zombie + 1 && ma_aggro > citizens_aggro[i]) {
                // 마동석의 어그로가 더 높으면 마동석이 공격받고 게임이 끝나지 않음
                temp_stamina = stamina;
                stamina -= 1;
                printf("Zombie attacked madongseok (aggro %d vs %d, madongseok stamina: %d -> %d)\n", citizens_aggro[i], ma_aggro, temp_stamina, stamina);
                if (stamina <= 0) {
                    printf("madongseok die...\n");
                    bad_ending();
                }
            }
            else {
                // 마동석이 옆에 없거나 그의 어그로가 낮은 경우 시민이 물림
                printf("citizen%d has been attacked by zombie\n", i);
            }
        }
    }

    // 남은 시민 출력
    printf("Remaining citizens: %d\n", num_citizens);
    }
   


// 시민이 탈출했을 때
//결과 출력
void result() {
    int zombie_attack = 0; // 좀비의 공격 여부를 나타내는 변수

    for (int i = 0; i < num_citizens; i++) {
        if (zombie == citizens[i] + 1) {
            printf("zombie caught citizen%d\n", i);
            citizens[i] = -1; // 물린 시민을 기차에서 없앰
            zombie_attack = 1; // 좀비가 공격했음을 표시
            break; // 한 번 물었으면 더 이상 루프를 돌지 않음
        }
        if(zombie == citizen + 1) {
            printf("zombie caught citizen");
            citizen = -1;
            zombie_attack = 1;
            break;
        }
    }

    if (!zombie_attack) {
        printf("zombie attacked nobody.\n");
    }
    printf("citizen does nothing.\n");
}

void madongseok_catch() {
    if (zombie == madongseok - 1) {
        printf("zombie attacked madongseok.\n");
        temp_stamina = stamina;
        stamina -= 1;
        printf("스태미나: %d -> %d\n", temp_stamina, stamina);
    }
}
//마동석의 스테미나 0
void ma_stamina_zero() {
    if (stamina == 0) {
        printf("madongseok die...");
        bad_ending();
        exit(1);
    }
}
//메인함수
int main() {
    srand((unsigned int)time(NULL));
    print_intro();
    input_train_length();
    num_citizen_random();
    madonseok_stamina();
    input_probability();

    // 초기 열차 상태 출력
    print_train();
    printf("\n\n");

    while (1) {
        turn++;

        // 시민과 좀비의 움직임 계산
        move_citizens(); //시민들 움직임 계산
        move_zombie(); // 좀비의 움직임 계산

        // 첫 번째 기차 상태 출력 (시민과 좀비 이동 후)
        print_train(); // 열차 상태 출력
        printf("\n");

        // 시민과 좀비의 이동 상황을 출력;
        print_move_citizens(); //시민들 이동 상황 출력
        print_move_zombie(); // 좀비의 이동 상황 출력
        printf("\n");

        // 마동석의 이동 계산을 먼저 수행
        ma_move();
        printf("\n");
        // 두 번째 기차 상태 출력 (마동석 이동 후)
        print_train(); // 열차 상태 출력
        printf("\n");

        // 마동석의 이동 상황 출력
        print_ma_move();
        printf("\n");

        // 결과 출력
        result();
        zombie_catch_citizens();
        printf("\n");

        //마동석이 좀비한테 물렸을때
        madongseok_catch();

        // 마동석의 행동 결정
        ma_action();

        //마동석이 쉴때
        ma_action_rest();

        //마동석이 도발할때
        ma_action_provoke();

        //마동석이 좀비를 당길때
        ma_action_pull();



        // 좀비에게 잡혔는지 확인


        //마동석의 스테미나가 0이 되었을때
        ma_stamina_zero();
    }

    return 0;
}