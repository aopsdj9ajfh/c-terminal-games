#pragma once

#define MAX_INVENTORY 10
#define MAX_BQUIP_BAG 8
#define SAVE_FILE "rpg_save_bin"

typedef enum {
    ITEM_HP_POTION = 1,
    ITEM_MP_POTION = 2,
    ITEM_WEAPON = 3,
    ITEM_ARMOR = 4
} ItemType;

typedef struct {
    char name[20];
    ItemType type;
    int value;
    int count;
} Item;

typedef struct {
    char weaponName[20];
    int weaponAtk;
    char armorName[20];
    int armorDef;
} Equip;

typedef struct {
    char name[20];
    int max_hp;
    int hp;
    int max_mp;
    int mp;
    int atk;
    int def;
    Equip equip;
    Item inv[MAX_INVENTORY];
    int item_cnt;
    int level;
    int exp;
    int expNeed;
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
    STATE_BQUIP_MGR,
    STATE_EXIT
} GameState;

typedef enum {
    EQ_WEAPON,
    EQ_ARMOR
} EquipType;

void InitPlayer(Player* p, const char* playerName);
void InitMoster(Moster* m, int level);

void ShowPlayerInfo(Player* p);
void ShowMosterInfo(Moster* m);
void ShowEquip(Player* p);

void WearEquip(Player* p, int idx);
void UnwearEquip(Player* p, EquipType* eqtype);

void ShowInventory(Player* p);
void OpenInventory(Player* p);

int  NormalAttack(Player* p,Moster* m);
int  CounterAttack(Player* p, Moster* m);
int  BattleTurn(Player* p, Moster* m, GameState* gamestate);
int  IsEscape();
void UseItem(Player* p, int idx);

void AddHpPotion(Player* p);
void AddMpPotion(Player* p);
void RandomDropEquip(Player* p, int level);

void CheckLevelUp(Player* p);
int  GetMosterExp(int level);

int  SaveGame(Player* p);
int  LoadGame(Player* p);
void GameRun();