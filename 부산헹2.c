#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//기차열
#define LEN_MIN 15
#define LEN_MAX 50
//확률
#define PROB_MIN 10
#define PROB_MAX 90
//어그로
#define AGGRO_MIN 0
#define AGGRO_MAX 5
//마동석 이동 방향
#define MOVE_LEFT 1
#define MOVE_STAY 0
//좀비의 공격 대상
#define ATK_NONE 0
#define ATK_CITIZEN 1
#define ATK_DONGSEOK 2
//마동석 행동
#define ACTION_REST 0
#define ACTION_PROVOKE 1
#define ACTION_PULL 2

int train_length, madongseok, zombie, citizen, turn = 1, percentile_probability;
int temp_aggro, stamina, move, action, temp_citizen, temp_zombie, temp_aggro;
int citizen_aggro = 1;
int ma_aggro = 1;

// 인트로 함수
void print_intro() {
    printf("********************\n");
    printf("**  부산헹 게임   **\n");
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

//마동석 스테미나 함수
int madonseok_staminia() {
    printf("\n");
    printf("마동석 스테미나 (0~5 >>)");
    scanf("%d", &stamina);
    printf("\n");

    if (stamina < AGGRO_MIN || stamina > AGGRO_MAX) {
        madonseok_staminia();
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
    int rcitizen = rand() % 101;  // temp_citizen을 static 변수로 선언하고 초기화
    if (100 - percentile_probability >= rcitizen) {
        citizen -= 1; // 시민을 움직임
    }
}

void print_move_citizen() {
    if (temp_citizen == citizen) {
        temp_aggro = citizen_aggro;
        citizen_aggro -= 1;
        if (citizen_aggro < AGGRO_MIN) citizen_aggro = AGGRO_MIN;
        printf("citizen: stay %d (aggro %d -> %d)\n", citizen, temp_aggro, citizen_aggro);
    } else {
        temp_aggro = citizen_aggro;
        citizen_aggro += 1;
        if (citizen_aggro > AGGRO_MAX) citizen_aggro = AGGRO_MAX;
        printf("citizen: %d -> %d (aggro: %d -> %d)\n", temp_citizen, citizen, temp_aggro, citizen_aggro);
    }
}

// 좀비 이동 함수
void move_zombie() {
    temp_zombie = zombie;
    int rzom = rand() % 101;
    if (turn % 2 != 0) { // 홀수 턴일 때만 좀비가 움직임
        if (100 - percentile_probability <= rzom) {
            zombie -= 1; // 좀비를 움직임
        }
    }
}
void print_move_zombie() {
if (turn % 2 != 0) {
        if (temp_zombie == zombie) {
            printf("zombie: stay %d\n", zombie);
        } else {
            printf("zombie: %d -> %d\n", temp_zombie, zombie);
        }
    } else {
        printf("zombie: stay %d (cannot move)\n", zombie);
    }
}
// 마동석 이동 함수
void ma_move() {
    printf("madongseok move (0: stay, 1: left) >> ");
    scanf("%d", &move);
    if (move == MOVE_STAY) {
        temp_aggro = ma_aggro;
        ma_aggro -= 1;
        if (ma_aggro < AGGRO_MIN) ma_aggro = AGGRO_MIN;
        printf("stay %d (aggro: %d -> %d, stamina: %d)\n", madongseok, temp_aggro, ma_aggro, stamina);
    }
    if (move == MOVE_LEFT && madongseok - 1 != zombie) {
        temp_aggro = ma_aggro;
        madongseok -= 1;
        ma_aggro += 1;      
    }else{
        printf("left %d (aggro: %d -> %d, stamina: %d)\n", madongseok, temp_aggro, ma_aggro, stamina);
}
}

// 마동석 행동 함수
void ma_action() {
    int temp_stamina = stamina;
    printf("madongseok action (0: rest, 1: provoke) >> ");
    scanf("%d", &action);

    if (action == ACTION_REST) {
        printf("\n");
        printf("madongseok rests...\n");
        stamina += 1;
        if (stamina > AGGRO_MAX) stamina = AGGRO_MAX;
        printf("madongseok: %d (aggro: %d -> %d, stamina: %d -> %d)\n", madongseok, temp_aggro, ma_aggro, temp_stamina, stamina);
        ma_aggro -= 1;
        if (ma_aggro < AGGRO_MIN) ma_aggro = AGGRO_MIN;
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

int main() {
    srand((unsigned int)time(NULL));
    print_intro();
    input_train_length();
    madonseok_staminia();
    input_probability();
    // 초기 열차 상태 출력
    print_train();
    printf("\n\n");
    while (1) {
        move_citizen();
        move_zombie();
        printf("\n");
        print_train();
        printf("\n");
        print_move_citizen();
        print_move_zombie();
        ma_move();
        print_train();
        printf("\n");
        ma_action();
        // 좀비에게 잡힘
        zombie_catch();
        // 시민 탈출
        citizen_escape();
        turn++;
    }
    return 0;
}
