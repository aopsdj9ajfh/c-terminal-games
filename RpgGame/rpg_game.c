#include "rpg_game.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <string.h>
#include <time.h>

void InitPlayer(Player* p, const char* playerName){
    memset(p, 0, sizeof(*p));
    strcpy(p->name, playerName);
    p->max_hp = 100;
    p->hp = 100;
    p->max_mp = 50;
    p->mp = 50;
    p->atk = 25;
    p->def = 8;

    strcpy(p->equip.weaponName, "无");
    p->equip.weaponAtk = 0;
    strcpy(p->equip.armorName, "无");
    p->equip.armorDef = 0;

    p->item_cnt = 0;
    p->level = 1;
    p->exp = 0;
    p->expNeed = 80;

    strcpy(p->inv[0].name, "回血药剂");
    p->inv[0].count = 2;
    p->inv[0].type = 1;
    p->inv[0].value = 35;
    strcpy(p->inv[1].name, "回蓝药剂");
    p->inv[1].count = 1;
    p->inv[1].type = 2;
    p->inv[1].value = 25;

    p->item_cnt = 2;
}

void InitMoster(Moster* m, int level){
    memset(m, 0, sizeof(m));
    if (level == 1){
        strcpy(m->name, "哥布林");
        m->max_hp = 70;
        m->hp = 70;
        m->atk = 18;
        m->def = 3;
    }else if(level == 2){
        strcpy(m->name, "兽人战士");
        m->max_hp = 120;
        m->hp = 120;
        m->atk = 28;
        m->def = 8;
    }else{
        strcpy(m->name, "史莱姆之王");
        m->max_hp = 220;
        m->hp = 220;
        m->atk = 40;
        m->def = 12;
    }
}

void ShowEquip(Player* p){
    printf("【穿戴】武器%s (+%d攻击) | 护甲%s (+%d防御)\n", 
        p->equip.weaponName, p->equip.weaponAtk, p->equip.armorName, p->equip.armorDef);
}

void WearEquip(Player* p, int idx){
    Item* it = &p->inv[idx];
    if (it->type == ITEM_WEAPON){
        //把旧武器返回背包
        if (strcmp(p->equip.weaponName, "无") != 0){
            strcpy(p->inv[p->item_cnt].name, p->equip.weaponName);
            p->inv[p->item_cnt].type = ITEM_WEAPON;
            p->inv[p->item_cnt].value = p->equip.weaponAtk;
            p->inv[p->item_cnt].count = 1;
            p->item_cnt++;
        }
        //装备新武器
        strcpy(p->equip.weaponName, it->name);
        p->equip.weaponAtk = it->value;
        it->count--;
        printf("✅成功装备武器%s!\n", it->name);
    }else if (it->type == ITEM_ARMOR){
        if (strcmp(p->equip.armorName, "无") != 0){
            p->inv[p->item_cnt].type = ITEM_ARMOR;
            p->inv[p->item_cnt].value = p->equip.armorDef;
            p->inv[p->item_cnt].count = 1;
            p->item_cnt++;
        }
        strcpy(p->equip.armorName, it->name);
        p->equip.armorDef = it->value;
        it->count--;
        printf("✅成功装备护具%s!\n", it->name);
    }
}

void UnWearEquip(Player* p, EquipType* eqtype){
    char* eqName;
    int* eqVal;
    int itemType;
    const char* msgName;
    if (eqtype == EQ_WEAPON){
        eqName = p->equip.weaponName;
        eqVal = &p->equip.weaponAtk;
        itemType = ITEM_WEAPON;
        msgName = "武器";
    }else{
        eqName = p->equip.armorName;
        eqVal = &p->equip.armorDef;
        itemType = ITEM_ARMOR;
        msgName = "护甲";
    }
    
    if (strcmp(eqName, "无") == 0) {printf("没有装备%s\n", msgName); return;}
    if (p->item_cnt >= MAX_INVENTORY) {printf("背包已满，无法卸下\n"); return;}

    strcpy(p->inv[p->item_cnt].name, eqName);
    p->inv[p->item_cnt].type = itemType;
    p->inv[p->item_cnt].value = *eqVal;
    p->inv[p->item_cnt].count = 1;
    p->item_cnt++;

    strcpy(eqName, "无");
    *eqVal = 0;
    printf("已卸下%s", msgName);
}

void ShowPlayerInfo(Player* p){
    int realAtk = p->atk + p->equip.weaponAtk;
    int realDef = p->def + p->equip.armorDef;
    printf("\n======玩家%s======\n", p->name);
    printf("生命: %d / %d\n", p->hp, p->max_hp);
    printf("魔法: %d / %d\n", p->mp, p->max_mp);
    printf("攻击: %d (基础%d + %d)\n",realAtk, p->atk, p->equip.weaponAtk);
    printf("防御: %d (基础%d + %d)\n",realDef, p->def, p->equip.armorDef);
    printf("经验: %d / %d \n", p->exp, p->expNeed);
    ShowEquip(p);
}

void ShowMosterInfo(Moster* m){
    printf("\n######怪物%s#######\n", m->name);
    printf("生命: %d / %d\n", m->hp, m->max_hp);
    printf("攻击: %d  防御: %d\n", m->atk, m->def);
}

void ShowInventory(Player* p){
    printf("\n------背包-------\n");
    for (int i = 0; i < p->item_cnt; i++){
        char typestr[16] = {0};
        if (p->inv[i].count > 0){
            if (p->inv[i].type == ITEM_HP_POTION) strcpy(typestr, "【回血药】");
            if (p->inv[i].type == ITEM_MP_POTION) strcpy(typestr, "【回蓝药】");
            if (p->inv[i].type == ITEM_WEAPON)    strcpy(typestr, "【武器】");
            if (p->inv[i].type == ITEM_ARMOR)     strcpy(typestr, "【护甲】");
            printf("[%d]%s 名称:%s 数量:%d 属性:+%d",i, typestr, p->inv[i].name, p->inv[i].count, p->inv[i].value);
        }
    }
    printf("---------------\n");
}

void AddHpPotion(Player* p){
    strcpy(p->inv[p->item_cnt].name,"回血药剂");
    p->inv[p->item_cnt].count = 1;
    p->inv[p->item_cnt].type = ITEM_HP_POTION;
    p->inv[p->item_cnt].value = 35;
    p->item_cnt++;
}

void AddMpPotion(Player* p){
    strcpy(p->inv[p->item_cnt].name,"回蓝药剂");
    p->inv[p->item_cnt].count = 1;
    p->inv[p->item_cnt].type = ITEM_MP_POTION;
    p->inv[p->item_cnt].value = 25;
    p->item_cnt++;
}

void RandomDropEquip(Player* p, int level){
    if (p->item_cnt >= MAX_INVENTORY) return;
    if (rand()%100 > 40) return;

    Item* drop = &p->inv[p->item_cnt];
    if (rand()%2 == 0) {
        if (level == 1) {
            strcpy(drop->name, "破旧长矛");
            drop->value = 5;
        }else if (level == 2){
            strcpy(drop->name, "狼牙棒槌");
            drop->value = 10;
        }else {
            strcpy(drop->name, "皇家粘液刃");
            drop->value = 25;
        }drop->type = ITEM_WEAPON;
    }else {
        if (level == 1) {
            strcpy(drop->name, "粗布皮甲");
            drop->value = 3;
        }else if (level == 2){
            strcpy(drop->name, "兽皮战甲");
            drop->value = 6;
        }else {
            strcpy(drop->name, "粘液王胸甲");
            drop->value = 12;
        }drop->type = ITEM_ARMOR;
    }
    drop->count = 1;
    printf("🎁掉落装备: %s", drop->name);
    p->item_cnt++;
}

void UseItem(Player* p, int idx){
    if (idx < 0 || idx >= p->item_cnt || p->inv[idx].count <= 0){
        printf("无效道具\n");
        return;
    }
    Item* it = &p->inv[idx];
    switch (it->type){
        case ITEM_HP_POTION: int addhp = (p->hp + it->value) < (p->max_hp)? it->value: p->max_hp - p->hp; p->hp += addhp; printf("使用%s, 恢复%d生命\n", it->name, addhp); break;
        case ITEM_MP_POTION: int addmp = (p->mp + it->value) < (p->max_mp)? it->value: p->max_mp - p->mp; p->mp += addmp; printf("使用%s, 恢复%d魔力\n", it->name, addmp); break;
        case ITEM_WEAPON:case ITEM_ARMOR: WearEquip(p, idx); break;
        default: printf("无法使用的道具\n"); break;
    }
    it->count--;
}

int NormalAttack(Player* p, Moster* m){
    int crit = 0;
    int damage;
    int datk = p->atk + p->equip.weaponAtk;
    if (rand()%100 < 20) {
        printf("💥触发暴击！\n");
        crit = 1;
        damage = datk*2 - m->def;
    }else{
        damage = datk - m->def;
    }
    if (damage < 1) damage = 1;
    m->hp -= damage;
    printf("你对怪物造成%d点伤害\n", damage);
    if (m->hp < 0) {
        printf("✅你击败了怪物，战斗胜利！\n");
        return 1;
    }
    return 0;
}

void OpenInventory(Player* p){
    ShowInventory(p);
    int idx;
    printf("输入使用道具编号，否则-1返回战斗: ");
    scanf("%d", &idx);
    if (idx != -1){
        UseItem(p, idx);
    }
}

int IsEscape(){
    if (rand()%2 == 0) {
        printf("逃跑成功\n");
        return 1;
    }else{
        printf("逃跑失败\n");
        return 0;
    }
}

int CounterAttack(Player* p, Moster* m){
    int realDef = p->def + p->equip.armorDef;
    int mDamage = m->atk - realDef;
    if (mDamage < 1) mDamage = 1;
    p->hp -= mDamage;
    printf("👹%s对你造成了%d伤害,", m->name, mDamage);
    Sleep(600);
    if (p->hp < 0){
        printf("\n💀你被怪物打倒，游戏结束！\n");
        return 1;
    }
    return 0;
}

int BattleTurn(Player* p, Moster* m, GameState* gamestate){
    int choice;
    printf("\n------战斗回合------\n");
    ShowPlayerInfo(p);
    ShowMosterInfo(m);
    printf("\n1-普通攻击 | 2-打开背包 | 3-尝试逃跑\n");
    printf("请选择行动:");
    scanf("%d", &choice);
    switch(choice){
        case 1: if(NormalAttack(p, m) == 1) return 1; break;
        case 2: OpenInventory(p); break;
        case 3: if (IsEscape()) return 3; break;
    }
    if(CounterAttack(p, m) == 1) return -1;
    return 0;
}

int GetMosterExp(int level){
    switch(level){
        case 1: return 40; break;
        case 2: return 80; break;
        default: return 160; break;
    }
}

void CheckLevelUp(Player* p){
    while(p->exp >= p->expNeed){
        p->exp -= p->expNeed;
        p->level++;

        p->max_hp += 20;
        p->hp = p-> max_hp;
        p->max_mp += 10;
        p->mp = p->max_mp;
        p->atk += 6;
        p->def += 2;
        
        p->expNeed = (p->expNeed * 1.4);
        printf("\n🎉🎉恭喜升级, 当前等级: %d\n", p->level);
        printf("生命上限+20  魔法上限+10  攻击力+6  防御+2\n");
        Sleep(800);
    }
}

int SaveGame(Player* p){
    FILE* fp = fopen(SAVE_FILE, "wb");
    if (fp == NULL){
        printf("存档保存失败！");
        return 0;
    }
    fwrite(p, sizeof(*p), 1, fp);
    fclose(fp);
    fp = NULL;
    printf("💾存档保存成功！");
    return 1;
}

int LoadGame(Player* p){
    FILE* fp = fopen(SAVE_FILE, "rb");
    if (fp == NULL){
        printf("⚠没有存档文件\n");
        return 0;
    }
    size_t ret = fread(p, sizeof(*p), 1, fp);
    if (ret != 1) {
        printf("存档损坏！\n");
        return 0;
    }
    printf("📂存档读取成功！");
    return 1;
}

void GameRun(){
    srand((unsigned int)time(NULL));
    Player player;
    Moster moster;
    GameState gamestate = STATE_MAIN_MENU;
    int sel;

    while (gamestate != STATE_EXIT){
        if (gamestate == STATE_MAIN_MENU){
        printf("\n====文字RPG回合游戏====\n");
        printf("1-创建新角色\n");
        printf("2-读取存档继续游戏\n");
        printf("0-退出游戏\n");
        printf("请输入你的选择:");
        scanf("%d", &sel);
        if (sel == 1){
            char name[20];
            printf("请给角色命名: ");
            scanf("%s", name);
            InitPlayer(&player, name);
            gamestate = STATE_BATTLE;
        }else if (sel == 2){
            if (LoadGame(&player)){
                gamestate = STATE_BATTLE;
            }
        }else if(sel == 0){
            gamestate = STATE_EXIT;
            }
        }
        else if (gamestate == STATE_BATTLE){
            printf("\n请选择对手难度: 1.哥布林 2.兽人战士 3.史莱姆之王\n");
            printf("输入0返回主菜单 | 输入9保存游戏\n");
            printf("请输入你的选择:");
            scanf("%d", &sel);
            switch(sel){
                case 0: gamestate = STATE_MAIN_MENU; break; 
                case 9: SaveGame(&player); continue; 
                case 1:case 2:case 3:InitMoster(&moster, sel); break;
                default: printf("无效选择\n"); break;
            }
            int res = 0;
            while (res == 0){
                res = BattleTurn(&player, &moster, &gamestate);
            }
            if (res == 1){
                int expGet = GetMosterExp(sel);
                player.exp += expGet;
                printf("✨获得%d经验值!", expGet);
                CheckLevelUp(&player);
                RandomDropEquip(&player, sel);
                if (player.item_cnt < MAX_INVENTORY){
                    if (rand()%2 == 0) {AddHpPotion(&player); printf("🎁战利品：获得回血药剂\n");}
                    else {AddMpPotion(&player); printf("🎁战利品：获得回蓝药剂\n");}
                }
            }
            printf("\n按回车回到战斗选择画面");
            getchar();getchar();
        }
    }
    printf("游戏退出\n");
}