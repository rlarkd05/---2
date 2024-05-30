#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ������
#define LEN_MIN 15
#define LEN_MAX 50

// Ȯ��
#define PROB_MIN 10
#define PROB_MAX 90

// ��׷�
#define AGGRO_MIN 0
#define AGGRO_MAX 5

// ������ �̵� ����
#define MOVE_LEFT 1
#define MOVE_STAY 0

// ������ ���� ���
#define ATK_NONE 0
#define ATK_CITIZEN 1
#define ATK_DONGSEOK 2

// ������ �ൿ
#define ACTION_REST 0
#define ACTION_PROVOKE 1
#define ACTION_PULL 2
//���׹̳�
#define STM_MIN 0
#define STM_MAX 5
int train_length; //����
int madongseok; //������
int zombie, temp_zombie; //����
int citizen, temp_citizen; //�ù�
int turn = 0; //��
int percentile_probability; //Ȯ��
int temp_aggro; //�� ���
int stamina, temp_stamina; // ���׹̳� 
int move; //������ ������
int action; //������ �ൿ 
int citizen_aggro = 1; //�ù� ��׷� ó���� 1�� ����
int ma_aggro = 1; //������ ���� ó���� 1�� ���� 
int zombie_captured = 0; //���� ���������� ���θ� ��Ÿ���� ����
// ��Ʈ�� �Լ�
void print_intro() {
    printf("********************\n");
    printf("**  �λ��� ����   **\n");
    printf("********************\n");
    printf("���� ���ؼ� ������ Ż���ϼ���.\n");
    printf("����� ���ϴ�.\n");
    printf("\n");
    printf("������ �����մϴ�\n");
}

// Ȯ�� �Է� �Լ�
int input_probability() {
    printf("Ȯ���� �Է��ϼ���. (10 ~ 90) >> ");
    scanf_s("%d", &percentile_probability);
    printf("\n");
    //2-2 �Է°� ó��
    if (percentile_probability < PROB_MIN || percentile_probability > PROB_MAX) {
        input_probability();
    }

    return percentile_probability;
}

// ������ ���¹̳� �Լ�
int madonseok_stamina() {
    printf("\n");
    printf("������ ���¹̳� (0 ~ 5) >> ");
    scanf_s("%d", &stamina);
    printf("\n");

    if (stamina < STM_MIN || stamina > STM_MAX) {
        madonseok_stamina();
    }
    return stamina;
}

// ������ ���� �Է� �Լ�
void input_train_length() {
    printf("\n");
    printf("������ ���̸� �Է����ּ���. (15 ~ 50) >> ");
    scanf_s("%d", &train_length);
    printf("\n");
    //2-2 �Է°�
    if (train_length < LEN_MIN || train_length > LEN_MAX) {
        input_train_length();
    }

    madongseok = train_length - 2;
    zombie = train_length - 3;
    citizen = train_length - 6;
}

// ���� ���� ��� �Լ�
void print_train_line(char character, int train_length) {
    for (int i = 0; i < train_length; i++) {
        printf("%c", character);
    }
    printf("\n");
}
//����
void print_train() {
    print_train_line('#', train_length); // ù ��° �� ���
    for (int i = 0; i < train_length; i++) {
        if (i == citizen) {
            printf("C");
        }
        else if (i == madongseok) {
            printf("M");
        }
        else if (i == zombie) {
            printf("Z");
        }
        else if (i == 0 || i == train_length - 1) {
            printf("#");
        }
        else {
            printf(" ");
        }
    }
    printf("\n");
    print_train_line('#', train_length); // �� ��° �� ���
}

// �ù� �̵� �Լ�
void move_citizen() {
    temp_citizen = citizen;
    int rcitizen = rand() % 101;
    if (100 - percentile_probability >= rcitizen) {
        citizen -= 1; // �ù��� ������
    }
}
//�ù� �̵��Ѱ� ���
void print_move_citizen() {
    if (temp_citizen == citizen) {
        temp_aggro = citizen_aggro;
        citizen_aggro -= 1;
        if (citizen_aggro < AGGRO_MIN) {
            citizen_aggro = AGGRO_MIN;
        }
        printf("citizen: stay %d (aggro %d -> %d)\n", citizen, temp_aggro, citizen_aggro);
    }
    else {
        temp_aggro = citizen_aggro;
        citizen_aggro += 1;
        if (citizen_aggro > AGGRO_MAX) {
            citizen_aggro = AGGRO_MAX;
        }
        printf("citizen: %d -> %d (aggro: %d -> %d)\n", temp_citizen, citizen, temp_aggro, citizen_aggro);
    }
}

// ���� �̵� �Լ�
void move_zombie() {
    temp_zombie = zombie;
    // ���� ������ ������ ���� ����
    if (zombie_captured > 0) {
        zombie_captured--;
        return; // ���� ������ ������ �̵����� ����    
    }
    // Ȧ�� ���� ���� ���� ������
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
            // ���� ������ ������ �̵��� ���� ���¹̳��� ����
            if (zombie == madongseok - 1) {
                temp_stamina = stamina;
                stamina -= 1;
                printf("stamina: %d -> %d\n", temp_stamina, stamina);
            }
            // ���� �������� ��ġ���� Ȯ��
            if (zombie == madongseok) {
                // ��ġ�� �ʵ��� ���� ��ġ�� �ǵ�����
                zombie = temp_zombie;
            }
        }
        else {
            zombie = temp_zombie;
        }
    }
}
//���� ������ �ְų� �����̴� �κ�
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

// ������ �̵� �Լ�
void ma_move() {
    int temp_ma = madongseok;
    if (zombie != madongseok - 1) {
        do {
            printf("madongseok move (0: stay, 1: left) >> ");
            scanf_s("%d", &move);
        } while (move != MOVE_STAY && move != MOVE_LEFT);
    }
    else {
        do {
            printf("madongseok move (0: stay) >> ");
            scanf_s("%d", &move);
        } while (move != MOVE_STAY);
    }
    if (move == MOVE_STAY) {
        temp_ma = madongseok;
    }
    else {
        madongseok -= 1;
    }
}


//������ �̵� ��� �Լ�
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



// �������� ����
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
//�����Ҷ�
void ma_action_provoke() {
    if (action == ACTION_PROVOKE) {
        int temp_aggro = ma_aggro;
        int temp_stamina = stamina;

        printf("madongseok provoked zombie...\n");
        ma_aggro = AGGRO_MAX;
        printf("madongseok: %d (aggro: %d -> %d, stamina: %d)\n", madongseok, temp_aggro, ma_aggro, stamina);
    }
}
//��涧
void ma_action_pull() {
    int temp_aggro = ma_aggro;
    int mar = rand() % 101;
    int temp_stamina = stamina;
    if (action == ACTION_PULL) {
        printf("madongseok attempts to pull zombie...\n");
        if (100 - percentile_probability > mar) {
            printf("madongseok successfully pulled zombie! Zombie is captured for one turn.\n");
            ma_aggro += 2;
            stamina -= 1;
            zombie_captured = 1;  // ���� �� �� ���� ������
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
//������ �ൿ �Է�
void ma_action() {
    int temp_aggro = ma_aggro;
    if (temp_aggro > AGGRO_MAX) {
        temp_aggro = AGGRO_MAX;
    }
    if (madongseok == zombie + 1) {
        do {
            printf("madongseok action (0: rest, 1: provoke, 2: pull) >> ");//����� �پ������� pull ��
            scanf_s("%d", &action);
        } while (action != ACTION_REST && action != ACTION_PROVOKE && action != ACTION_PULL);
    }
    else {
        do {
            printf("madongseok action (0: rest, 1: provoke) >> ");//���� �پ����� ������ pull �ȶ�
            scanf_s("%d", &action);
        } while (action != ACTION_REST && action != ACTION_PROVOKE);
    }
}

// �ù� Ż�� �Լ�
void happy_ending() {
    printf("�ù��� Ż���߾��!!\n");
    printf("\n");
    printf("********************\n");
    printf("**   Game Clear!  **\n");
    printf("********************\n"); // �ƿ�Ʈ��
    exit(1);
}

// ���񿡰� �������� ������ �ƿ�Ʈ�� �Լ�
void bad_ending() {
    printf("\n");
    printf("********************\n");
    printf("**   Game Over..  **\n");
    printf("********************\n"); // �ƿ�Ʈ��
    exit(1);
}

// ���� �ù��� ����� ��
void zombie_catch() {
    if (citizen == zombie - 1) {
        if (madongseok == zombie + 1 && ma_aggro > citizen_aggro) {
            // �������� ��׷ΰ� �� ������ �������� ���ݹް� ������ ������ ����
            temp_stamina = stamina;
            stamina -= 1;
            printf("Zombie attacked madongseok (aggro %d vs %d, madongseok stamina: %d -> %d)\n", citizen_aggro, ma_aggro, temp_stamina, stamina);
            if (stamina <= 0) {
                printf("madongseok die...\n");
                bad_ending();
            }
        }
        else if (!(madongseok == zombie + 1)) {
            // �������� ���� ���� ��쿡�� ������ ����
            bad_ending();
        }
    }
}

// �ù��� Ż������ ��
void citizen_escape() {
    if (citizen == 1) {
        happy_ending();
    }
}
//��� ���
void result() {
    if (citizen != 1) {
        printf("citizen does nothing.");
    }
    if (zombie != madongseok - 1) {
        printf("\nzombie attacked nobody.");
    }
}
//�������� �������� ��������
void madongseok_catch() {
    if (zombie == madongseok - 1) {
        printf("zombie attacked madongseok.\n");
        temp_stamina = stamina;
        stamina -= 1;
        printf("���¹̳�: %d -> %d\n", temp_stamina, stamina);
    }
}
//�������� ���׹̳� 0
void ma_stamina_zero() {
    if (stamina == 0) {
        printf("madongseok die...");
        bad_ending();
        exit(1);
    }
}
//�����Լ�
int main() {
    srand((unsigned int)time(NULL));
    print_intro();
    input_train_length();
    madonseok_stamina();
    input_probability();

    // �ʱ� ���� ���� ���
    print_train();
    printf("\n\n");

    while (1) {
        turn++;

        // �ùΰ� ������ ������ ���
        move_citizen(); // �ù��� ������ ���
        move_zombie(); // ������ ������ ���

        // ù ��° ���� ���� ��� (�ùΰ� ���� �̵� ��)
        print_train(); // ���� ���� ���
        printf("\n");

        // �ùΰ� ������ �̵� ��Ȳ�� ���
        print_move_citizen(); // �ù��� �̵� ��Ȳ ���
        print_move_zombie(); // ������ �̵� ��Ȳ ���
        printf("\n");

        // �������� �̵� ����� ���� ����
        ma_move();

        // �� ��° ���� ���� ��� (������ �̵� ��)
        print_train(); // ���� ���� ���
        printf("\n");

        // �������� �̵� ��Ȳ ���
        print_ma_move();
        printf("\n");

        // ��� ���
        result();
        printf("\n");

        //�������� �������� ��������
        madongseok_catch();

        // �������� �ൿ ����
        ma_action();

        //�������� ����
        ma_action_rest();

        //�������� �����Ҷ�
        ma_action_provoke();

        //�������� ���� ��涧
        ma_action_pull();

        // ���񿡰� �������� Ȯ��
        zombie_catch();

        // �ù��� Ż���ߴ��� Ȯ��
        citizen_escape();

        //�������� ���׹̳��� 0�� �Ǿ�����
        ma_stamina_zero();
    }

    return 0;
}