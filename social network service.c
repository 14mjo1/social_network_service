#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define MAX_USERS 100
#define MAX_POSTS 10
#define MAX_FRIENDS 10
#define MAX_MESSAGES 10
#define MAX_LENGTH 100

typedef struct {
    char title[MAX_LENGTH];
    char content[MAX_LENGTH];
    int likes;
} Post;

typedef struct {
    char sender[MAX_LENGTH];
    char message[MAX_LENGTH];
} Message;

typedef struct {
    char id[MAX_LENGTH];
    char password[MAX_LENGTH];
    char name[MAX_LENGTH];
    char birthdate[MAX_LENGTH];
    char gender[MAX_LENGTH];
    char city[MAX_LENGTH];
    Post posts[MAX_POSTS];
    int friends[MAX_FRIENDS];
    Message messages[MAX_MESSAGES];
    int postCount;
    int friendCount;
    int messageCount;
} User;

User users[MAX_USERS];
int userCount = 0;
int loggedInUser = -1;

void clearScreen();
void setColor(int color);
void gotoXY(int x, int y);
void drawBox(int x, int y, int width, int height);
void loadUsers();
void saveUsers();
void signUp();
void viewPosts(int userId);
void writePost();
void signIn();
void signOut();
void viewProfile();
void addFriend();
void viewFriendsPosts();
void manageFriends();
void mainMenu();

enum ConsoleColors {
    CC_RED = 12,
    CC_GREEN = 10,
    CC_YELLOW = 14,
    CC_CYAN = 11,
    CC_MAGENTA = 13,
    CC_WHITE = 15
};

void clearScreen() {
    system("cls");
}

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void gotoXY(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void drawBox(int x, int y, int width, int height) {
    setColor(CC_WHITE);
    for (int i = 0; i < height; i++) {
        gotoXY(x, y + i);
        for (int j = 0; j < width; j++) {
            if (i == 0 || i == height - 1) {
                if (j == 0 || j == width - 1) {
                    printf("+");
                }
                else {
                    printf("-");
                }
            }
            else {
                if (j == 0 || j == width - 1) {
                    printf("|");
                }
                else {
                    printf(" ");
                }
            }
        }
    }
    setColor(CC_WHITE);
}

void loadUsers() {
    FILE* file = fopen("users.txt", "r");
    if (file == NULL) return;

    while (fread(&users[userCount], sizeof(User), 1, file)) {
        userCount++;
    }
    fclose(file);
}

void saveUsers() {
    FILE* file = fopen("users.txt", "w");
    if (file == NULL) return;

    for (int i = 0; i < userCount; i++) {
        fwrite(&users[i], sizeof(User), 1, file);
    }
    fclose(file);
}

void signUp() {
    if (userCount >= MAX_USERS) {
        clearScreen();
        drawBox(10, 5, 50, 3);
        gotoXY(12, 6); printf("�� �̻� ����ڸ� �߰��� �� �����ϴ�.\n");
        Sleep(2000);
        return;
    }

    User newUser;
    clearScreen();
    drawBox(10, 5, 50, 12);

    gotoXY(12, 6); printf("ID: ");
    gotoXY(12, 7); printf("Password: ");
    gotoXY(12, 8); printf("�̸�: ");
    gotoXY(12, 9); printf("�������(YYYYMMDD): ");
    gotoXY(12, 10); printf("����: ");
    gotoXY(12, 11); printf("����: ");

    gotoXY(16, 6); scanf("%s", newUser.id);
    gotoXY(22, 7); scanf("%s", newUser.password);
    gotoXY(18, 8); scanf("%s", newUser.name);
    gotoXY(32, 9); scanf("%s", newUser.birthdate);
    gotoXY(18, 10); scanf("%s", newUser.gender);
    gotoXY(18, 11); scanf("%s", newUser.city);

    newUser.postCount = 0;
    newUser.friendCount = 0;
    newUser.messageCount = 0;

    users[userCount++] = newUser;
    saveUsers();

    clearScreen();
    drawBox(10, 5, 50, 3);
    gotoXY(12, 6); printf("ȸ�������� �Ϸ�Ǿ����ϴ�!\n");
    Sleep(2000);
}

void viewPosts(int userId) {
    clearScreen();
    drawBox(10, 5, 50, 12);
    gotoXY(12, 6); printf("%s�� �Խñ�:\n", users[userId].name);
    for (int i = 0; i < users[userId].postCount; i++) {
        gotoXY(12, 7 + i * 2); printf("����: %s\n", users[userId].posts[i].title);
        gotoXY(12, 8 + i * 2); printf("����: %s\n", users[userId].posts[i].content);
    }
    gotoXY(12, 19); printf("���͸� ��������...");
    getchar();
    getchar();
}

void writePost() {
    if (loggedInUser == -1) {
        clearScreen();
        drawBox(10, 5, 50, 3);
        gotoXY(12, 6); printf("���� �α����ϼ���.\n");
        Sleep(2000);
        return;
    }

    if (users[loggedInUser].postCount >= MAX_POSTS) {
        clearScreen();
        drawBox(10, 5, 50, 3);
        gotoXY(12, 6); printf("�� �̻� �Խñ��� �߰��� �� �����ϴ�.\n");
        Sleep(2000);
        return;
    }

    clearScreen();
    drawBox(10, 5, 50, 12);
    gotoXY(12, 6); printf("�Խñ� ����: ");
    gotoXY(12, 8); printf("�Խñ� ����: ");

    Post newPost;
    gotoXY(27, 6); scanf(" %[^\n]", newPost.title);
    gotoXY(27, 8); scanf(" %[^\n]", newPost.content);
    newPost.likes = 0;

    users[loggedInUser].posts[users[loggedInUser].postCount++] = newPost;
    saveUsers();
    clearScreen();
    drawBox(10, 5, 50, 3);
    gotoXY(12, 6); printf("�Խñ��� ��ϵǾ����ϴ�.\n");
    Sleep(2000);
}

void signIn() {
    clearScreen();
    drawBox(10, 5, 50, 8);
    char id[MAX_LENGTH];
    char password[MAX_LENGTH];
    gotoXY(12, 6); printf("ID: ");
    gotoXY(12, 8); printf("Password: ");
    gotoXY(16, 6); scanf("%s", id);
    gotoXY(22, 8); scanf("%s", password);

    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].id, id) == 0 && strcmp(users[i].password, password) == 0) {
            loggedInUser = i;
            clearScreen();
            drawBox(10, 5, 50, 3);
            gotoXY(12, 6); printf("�α��� ����!\n");
            Sleep(2000);
            return;
        }
    }
    clearScreen();
    drawBox(10, 5, 50, 3);
    gotoXY(12, 6); printf("�α��� ����! ID �Ǵ� ��й�ȣ�� Ȯ���ϼ���.\n");
    Sleep(2000);
}

void signOut() {
    if (loggedInUser == -1) {
        clearScreen();
        drawBox(10, 5, 50, 3);
        gotoXY(12, 6); printf("���� �α����ϼ���.\n");
        Sleep(2000);
        return;
    }
    loggedInUser = -1;
    clearScreen();
    drawBox(10, 5, 50, 3);
    gotoXY(12, 6); printf("�α׾ƿ� �Ǿ����ϴ�.\n");
    Sleep(2000);
}

void viewProfile() {
    clearScreen();
    drawBox(10, 5, 50, 12);
    gotoXY(12, 6); printf("������:\n");
    gotoXY(12, 7); printf("ID: %s\n", users[loggedInUser].id);
    gotoXY(12, 8); printf("�̸�: %s\n", users[loggedInUser].name);
    gotoXY(12, 9); printf("�������: %s\n", users[loggedInUser].birthdate);
    gotoXY(12, 10); printf("����: %s\n", users[loggedInUser].gender);
    gotoXY(12, 11); printf("����: %s\n", users[loggedInUser].city);
    gotoXY(12, 12); printf("�Խñ� ��: %d\n", users[loggedInUser].postCount);
    gotoXY(12, 13); printf("ģ�� ��: %d\n", users[loggedInUser].friendCount);
    gotoXY(12, 14); printf("�޽��� ��: %d\n", users[loggedInUser].messageCount);
    gotoXY(12, 20); printf("���͸� ��������...");
    getchar();
    getchar();
}

void addFriend() {
    if (loggedInUser == -1) {
        clearScreen();
        drawBox(10, 5, 50, 3);
        gotoXY(12, 6); printf("���� �α����ϼ���.\n");
        Sleep(2000);
        return;
    }

    clearScreen();
    drawBox(10, 5, 50, 12);
    char friendId[MAX_LENGTH];
    gotoXY(12, 6); printf("�߰��� ģ���� ID: ");
    gotoXY(30, 6); scanf("%s", friendId);

    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].id, friendId) == 0) {
            users[loggedInUser].friends[users[loggedInUser].friendCount++] = i;
            saveUsers();
            clearScreen();
            drawBox(10, 5, 50, 3);
            gotoXY(12, 6); printf("ģ���� �߰��Ǿ����ϴ�.\n");
            Sleep(2000);
            return;
        }
    }
    clearScreen();
    drawBox(10, 5, 50, 3);
    gotoXY(12, 6); printf("�ش� ID�� ���� ����ڸ� ã�� �� �����ϴ�.\n");
    Sleep(2000);
}

void viewFriendsPosts() {
    if (loggedInUser == -1) {
        clearScreen();
        drawBox(10, 5, 50, 3);
        gotoXY(12, 6); printf("���� �α����ϼ���.\n");
        Sleep(2000);
        return;
    }

    clearScreen();
    drawBox(10, 5, 50, 12);
    gotoXY(12, 6); printf("ģ�� ���:\n");

    for (int i = 0; i < users[loggedInUser].friendCount; i++) {
        int friendId = users[loggedInUser].friends[i];
        gotoXY(12, 7 + i); printf("%d: %s\n", i + 1, users[friendId].name);
    }

    int choice;
    gotoXY(12, 12); printf("ģ���� �����ϼ��� (0: ���): ");
    gotoXY(41, 12); scanf("%d", &choice);

    if (choice == 0) return;
    if (choice > 0 && choice <= users[loggedInUser].friendCount) {
        viewPosts(users[loggedInUser].friends[choice - 1]);
    }
    else {
        clearScreen();
        drawBox(10, 5, 50, 3);
        gotoXY(12, 6); printf("�߸��� �����Դϴ�.\n");
        Sleep(2000);
    }
}

void manageFriends() {
    clearScreen();
    drawBox(10, 5, 50, 12);
    gotoXY(12, 6); printf("ģ�� ���:\n");

    for (int i = 0; i < users[loggedInUser].friendCount; i++) {
        int friendId = users[loggedInUser].friends[i];
        gotoXY(12, 7 + i); printf("%d: %s\n", i + 1, users[friendId].name);
    }

    gotoXY(12, 19); printf("1. ģ�� �߰�  2. ģ�� �Խñ� ����  3. ���ư���: ");
    int choice;
    gotoXY(60, 19); scanf("%d", &choice);

    switch (choice) {
    case 1:
        addFriend();
        break;
    case 2:
        viewFriendsPosts();
        break;
    case 3:
        return;
    default:
        clearScreen();
        drawBox(10, 5, 50, 3);
        gotoXY(12, 6); printf("�߸��� �����Դϴ�.\n");
        Sleep(2000);
    }
}

void mainMenu() {
    int choice;
    while (1) {
        clearScreen();
        drawBox(10, 5, 50, 12);

        if (loggedInUser == -1) {
            gotoXY(12, 6); printf("1. ȸ������\n");
            gotoXY(12, 7); printf("2. �α���\n");
            gotoXY(12, 8); printf("3. ����\n");
            gotoXY(12, 10); printf("����: ");
            gotoXY(20, 10); scanf("%d", &choice);

            switch (choice) {
            case 1: signUp(); break;
            case 2: signIn(); break;
            case 3: printf("\n\n\n\n\n"); exit(0);
            default:
                clearScreen();
                drawBox(10, 5, 50, 3);
                gotoXY(12, 6); printf("�߸��� �����Դϴ�.\n");
                Sleep(2000);
            }
        }
        else {
            gotoXY(12, 6); printf("1. �Խñ� �ۼ�\n");
            gotoXY(12, 7); printf("2. �� �Խñ� ����\n");
            gotoXY(12, 8); printf("3. ģ�� ���\n");
            gotoXY(12, 9); printf("4. �� ������ ����\n");
            gotoXY(12, 10); printf("5. �α׾ƿ�\n");
            gotoXY(12, 12); printf("����: ");
            gotoXY(20, 12); scanf("%d", &choice);

            switch (choice) {
            case 1: writePost(); break;
            case 2: viewPosts(loggedInUser); break;
            case 3: manageFriends(); break;
            case 4: viewProfile(); break;
            case 5: signOut(); break;
            default:
                clearScreen();
                drawBox(10, 5, 50, 3);
                gotoXY(12, 6); printf("�߸��� �����Դϴ�.\n");
                Sleep(2000);
            }
        }
    }
}

int main() {
    loadUsers();
    mainMenu();
    return 0;
}