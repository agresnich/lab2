#include <stdio.h>
#include <Windows.h>

#define INV_SIZE 10
#define HOURS_PER_DAY 24

#define ITEM_EMPTY   0
#define ITEM_WOOD    1
#define ITEM_STONE   2
#define ITEM_SEEDS   3
#define ITEM_WHEAT   4
#define ITEM_APPLE   5
#define ITEM_IRON    6
#define ITEM_WATER   7
#define ITEM_CLOTH   8
#define ITEM_TOOL    9

/* Безопасный ввод целого числа с проверкой */
int read_int(const char *prompt) {
    int value;
    char ch;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &value) == 1) {
            /* Съедаем остаток строки, чтобы не ломать следующий ввод */
            while ((ch = getchar()) != '\n' && ch != EOF);
            return value;
        }
        /* Очищаем буфер ввода */
        while ((ch = getchar()) != '\n' && ch != EOF);
        printf("Ошибка: нужно ввести целое число. Попробуйте снова.\n");
    }
}

const char* item_name(int id) {
    switch (id) {
        case ITEM_EMPTY: return "Пусто";
        case ITEM_WOOD:  return "Дерево";
        case ITEM_STONE: return "Камень";
        case ITEM_SEEDS: return "Семена";
        case ITEM_WHEAT: return "Пшеница";
        case ITEM_APPLE: return "Яблоко";
        case ITEM_IRON:  return "Железо";
        case ITEM_WATER: return "Вода";
        case ITEM_CLOTH: return "Ткань";
        case ITEM_TOOL:  return "Инструмент";
        default:         return "Неизвестно";
    }
}

void print_inventory(const int inv[INV_SIZE]) {
    printf("Инвентарь:\n");
    for (int i = 0; i < INV_SIZE; i++) {
        if (inv[i] == ITEM_EMPTY) {
            printf("  Слот %d: [%d]\n", i, inv[i]);
        } else {
            printf("  Слот %d: [%d] (%s)\n", i, inv[i], item_name(inv[i]));
        }
    }
}

void print_time(int day, int hour) {
    printf("Текущее время: День %d, %02d:00\n", day, hour);
}

/* Пункт 2: промотать время */
void advance_time(int *day, int *hour, int add_hours) {
    int total = *hour + add_hours;
    while (total >= HOURS_PER_DAY) {
        total -= HOURS_PER_DAY;
        (*day)++;
    }
    *hour = total;
}

/* Пункт 4: положить предмет */
void put_item(int inv[INV_SIZE]) {
    int slot = read_int("Введите индекс слота (0-9): ");
    if (slot < 0 || slot >= INV_SIZE) {
        printf("Ошибка: индекс слота вне диапазона [0..%d].\n", INV_SIZE - 1);
        return;
    }
    int id = read_int("Введите ID предмета (0-9): ");
    if (id < 0 || id > 9) {
        printf("Ошибка: ID должен быть от 0 до 9.\n");
        return;
    }
    inv[slot] = id;
    printf("В слот %d положен предмет [%d] (%s).\n", slot, id, item_name(id));
}

/* Пункт 5: выбросить предмет */
void drop_item(int inv[INV_SIZE]) {
    int slot = read_int("Введите индекс слота (0-9): ");
    if (slot < 0 || slot >= INV_SIZE) {
        printf("Ошибка: индекс слота вне диапазона [0..%d].\n", INV_SIZE - 1);
        return;
    }
    if (inv[slot] == ITEM_EMPTY) {
        printf("Слот %d уже пуст.\n", slot);
        return;
    }
    printf("Из слота %d выброшен предмет [%d] (%s).\n",
           slot, inv[slot], item_name(inv[slot]));
    inv[slot] = ITEM_EMPTY;
}

/* Пункт 6 (вариант 2): сортировка рюкзака (сжатие) */
void compact_inventory(int inv[INV_SIZE]) {
    printf("\n--- До сортировки ---\n");
    print_inventory(inv);

    int write = 0;
    for (int i = 0; i < INV_SIZE; i++) {
        if (inv[i] != ITEM_EMPTY) {
            inv[write++] = inv[i];
        }
    }
    while (write < INV_SIZE) {
        inv[write++] = ITEM_EMPTY;
    }

    printf("\n--- После сортировки ---\n");
    print_inventory(inv);
}

int main(void) {
    /* Один раз настраиваем консоль на UTF-8 — и всё */
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    int current_day  = 1;
    int current_hour = 8;

    int inventory[INV_SIZE] = {
        ITEM_WOOD, ITEM_STONE, ITEM_SEEDS, ITEM_EMPTY, ITEM_APPLE,
        ITEM_EMPTY, ITEM_WOOD, ITEM_IRON, ITEM_EMPTY, ITEM_TOOL
    };

    int running = 1;
    while (running) {
        printf("\n===== МЕНЮ =====\n");
        printf("[0] Выход\n");
        printf("[1] Посмотреть на часы\n");
        printf("[2] Промотать время (Поработать)\n");
        printf("[3] Посмотреть инвентарь\n");
        printf("[4] Положить предмет в слот\n");
        printf("[5] Выбросить предмет\n");
        printf("[6] Сортировка рюкзака (вариант)\n");

        int choice = read_int("Выбор: ");

        switch (choice) {
            case 0:
                printf("Выход. Хорошего дня, фермер!\n");
                running = 0;
                break;
            case 1:
                print_time(current_day, current_hour);
                break;
            case 2: {
                int h = read_int("Сколько часов работать? ");
                if (h < 0) {
                    printf("Ошибка: часы не могут быть отрицательными.\n");
                    break;
                }
                advance_time(&current_day, &current_hour, h);
                print_time(current_day, current_hour);
                break;
            }
            case 3:
                print_inventory(inventory);
                break;
            case 4:
                put_item(inventory);
                break;
            case 5:
                drop_item(inventory);
                break;
            case 6:
                compact_inventory(inventory);
                break;
            default:
                printf("Неверный пункт меню. Повторите ввод.\n");
                break;
        }
    }
    return 0;
}