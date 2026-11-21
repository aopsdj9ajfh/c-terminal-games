#include "rpg_game.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <string.h>
#include <time.h>

void InitPlayer(Player* p, const char* playerName){
    memset(p, 0, sizeof(p));
    strcpy(p->name, playerName);
    p->max_hp = 100;
    p->hp = 100;
    p->max_mp = 50;
    p->mp = 50;
    p->atk = 25;
    p->def = 8;
    p->item_cnt = 0;

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

void ShowPlayerInfo(Player* p){
    printf("\n======玩家%s======\n", p->name);
    printf("生命: %d / %d\n", p->hp, p->max_hp);
    printf("魔法: %d / %d\n", p->mp, p->max_mp);
    printf("攻击: %d  防御: %d\n", p->atk, p->def);
}

void ShowMosterInfo(Moster* m){
    printf("\n######怪物%s#######\n", m->name);
    printf("生命: %d / %d\n", m->hp, m->max_hp);
    printf("攻击: %d  防御: %d\n", m->atk, m->def);
}

void ShowInventory(Player* p){
    printf("\n------背包-------\n");
    for (int i = 0; i < p->item_cnt; i++){
        if (p->inv[i].count > 0){
            printf("[%d] 名称:%s 数量:%d 效果:恢复%d\n",i, p->inv[i].name, p->inv[i].count, p->inv[i].value);
        }
    }
    printf("---------------\n");
}

void AddHpPotion(Player* p){
    strcpy(p->inv[p->item_cnt].name,"回血药剂");
    p->inv[p->item_cnt].count = 1;
    p->inv[p->item_cnt].type = 1;
    p->inv[p->item_cnt].value = 35;
    p->item_cnt++;
}

void AddMpPotion(Player* p){
    strcpy(p->inv[p->item_cnt].name,"回蓝药剂");
    p->inv[p->item_cnt].count = 1;
    p->inv[p->item_cnt].type = 2;
    p->inv[p->item_cnt].value = 25;
    p->item_cnt++;
}

void UseItem(Player* p, int idx){
    if (idx < 0 || idx >= p->item_cnt || p->inv[idx].count < 0){
        printf("无效道具\n");
        return;
    }
    Item* it = &p->inv[idx];
    switch (it->type){
        case 1: int addhp = (p->hp + it->value) < (p->max_hp)? it->value: p->max_hp - p->hp; p->hp += addhp; printf("使用%s, 恢复%d生命\n", it->name, addhp); break;
        case 2: int addmp = (p->mp + it->value) < (p->max_mp)? it->value: p->max_mp - p->mp; p->mp += addmp; printf("使用%s, 恢复%d魔力\n", it->name, addmp); break;
        default: printf("无法使用的道具\n"); break;
    }
    it->count--;
}

int NormalAttack(Player* p, Moster* m){
    int crit = 0;
    int damage;
    if (rand()%100 < 20) {
        printf("💥触发暴击！\n");
        crit = 1;
        damage = p->atk*2 - m->def;
    }else{
        damage = p->atk - m->def;
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
    int mDamage = m->atk - p->def;
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
            player.hp = player.max_hp;
            if (res == 1){
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