#pragma once

#define MAX_INVENTORY 10
#define SAVE_FILE "rpg_save_bin"

typedef struct {
    char name[20];
    int type;
    int value;
    int count;
} Item;

typedef struct {
    char name[20];
    int max_hp;
    int hp;
    int max_mp;
    int mp;
    int atk;
    int def;
    Item inv[MAX_INVENTORY];
    int item_cnt;
} Player;

typedef struct {
    char name[20];
    int max_hp;
    int hp;
    int atk;
    int def;
} Moster;

typedef enum {
    STATE_MAIN_MENU,
    STATE_BATTLE,
    STATE_INVENTORY,
    STATE_EXIT
} GameState;

void InitPlayer(Player* p, const char* playerName);
void InitMoster(Moster* m, int level);

void ShowPlayerInfo(Player* p);
void ShowMosterInfo(Moster* m);

void ShowInventory(Player* p);
void OpenInventory(Player* p);

int  NormalAttack(Player* p,Moster* m);
int  CounterAttack(Player* p, Moster* m);
int  BattleTurn(Player* p, Moster* m, GameState* gamestate);
int  IsEscape();
void UseItem(Player* p, int idx);

void AddHpPotion(Player* p);
void AddMpPotion(Player* p);

int  SaveGame(Player* p);
int  LoadGame(Player* p);
void GameRun();