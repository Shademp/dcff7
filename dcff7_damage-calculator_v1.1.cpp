#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cmath>
#include <Windows.h>

#define FE_TOWARDZERO

using namespace std;

// Sets code_page to allow Japanese symbols in the CMD.
// Source "Adrian McCarthy" on Stack Overflow: https://stackoverflow.com/questions/23471873/change-console-code-page-in-windows-c
class UTF8CodePage {
public:
    UTF8CodePage() : m_old_code_page(::GetConsoleOutputCP()) {
        ::SetConsoleOutputCP(CP_UTF8);
    }
    ~UTF8CodePage() { ::SetConsoleOutputCP(m_old_code_page); }

private:
    UINT m_old_code_page;
};


union attackpowercoefficient {
    int hexrepresentation = 0x3F400000;
    float powerfloat; //0.75
}physicalcoefficient;

float powerconstant = 0; //This variable exists in the game data but never impacts the damage outcome.

union magicpowercoefficient {
    int hexrepresentation = 0x3E8A3D71;
    float magicfloat; //0.270000010729
}magiccoefficient;

union magicpowerconstant {
    int hexrepresentation = 0x3FB33333;
    float magicfloat; //1.39999997616
}magicconstant;

union defensecoefficient {
    int hexrepresentation = 0x3EA8F5C3;
    float defensefloat; //0,330000013113
}defcoefficient;

union defenseconstant {
    int hexrepresentation = 0x3ECCCCCD;
    float defensefloat; //0,40000000596
}defconstant;

union coefficientforpercentage {
    int hexrepresentation = 0x3C23D70A;
    float zeropointzeroone; //0.00999999977648
}dividebyonehundred;

union hitboxmodifier {
    int hexrepresentation = 0x3D4CCCCD;
    float hitboxfloat; //0.0500000007451
}hitboxmodifier;


int vincentHP[51] = { 0,780,860,940,1020,1100,1180,1260,1340,1420,1500,1550,1600,1650,1700,1750,1800,1850,1900,1950,2000,2040,2080,2120,2160,2200,2240,2280,2320,2360,2400,2430,2460,2490,2520,2550,2580,2610,2640,2670,2700,2730,2760,2790,2820,2850,2880,2910,2940,2970,3000 };
int vincentSTR[51] = { 0,2,4,6,8,10,12,14,16,18,20,23,26,29,32,35,38,41,44,47,50,52,54,56,58,60,62,64,66,68,70,72,74,76,78,80,82,84,86,88,90,91,92,93,94,95,96,97,98,99,100 };
int vincentDEF[51] = { 0,4,8,12,16,20,24,28,32,36,40,42,44,46,48,50,52,54,56,58,60,62,64,66,68,70,72,74,76,78,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100 };
int vincentDEX[51] = { 0,2,5,8,11,14,18,21,24,27,30,32,34,36,38,40,42,44,46,48,50,52,54,56,58,60,62,64,66,68,70,72,74,76,78,80,82,84,86,88,90,91,92,93,94,95,96,97,98,99,100 };
int vincentINT[51] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,22,24,26,28,30,32,34,36,38,40,43,46,49,52,55,58,61,64,67,70,73,76,79,82,85,88,91,94,97,100 };


bool statinfodump, manainfodump, enemymagicdefenseinfodump, bossinfodump;
int selection, selectedboss, selectingenemymagicdefense;
int temporarystorage;

int difficultymode;
int vincentlevel, bullet_type, handgun, machinegun, rifle, barrel, powerbooster, magic, manabooster, killchain;
int framepower, barrelpower, totalatkpower;

int baseSTRpower, baseINTpower, baseDEFpower;
int chaindamage, totalspellpower, preliminarydamage;

int damagelimiter = 0;
int magicdamagelimiter = 0;
float defensemultiplier[5] = { 1.0, 0, 0.25, 0.5, 2.0 }; //Unused variable for now. Same element order as the variables show up in game memory.

int language; //0 = English. 1 = Japanese. Used in switch statements for cout and for string arrays.

uint16_t menu_default;


string enemymagicdefense[2][3] = { { "Normal Damage","1/2 Damage","1/4 Damage" },
u8"ノーマルダメージ",u8"1/2ダメージ",u8"1/4ダメージ" };

string stockphrases_selection[2][2] = { {"Selection: ","You have selected: "},
u8"選択： ",u8"選択： " };

string stockphrases_error[2][1] = { {"Invalid input. Please try again."},
u8"無効入力。 もう一度やり直してください。" };

string stockphrases_bullets[2][4] = { {"1 bullet: ","2 bullets: ","3 bullets: ","Final damage: "},
u8"弾丸1： ",u8"弾丸2： ",u8"弾丸3： ",u8"最終ダメージ： " };

string stockphrases_damagecaps[2][3] = { {". Capped at 9999.",". Capped at 5000.",". Capped at 2500."},
u8"。 上限は9999です。",u8"。 上限は5000です。",u8"。 上限は2500です。" };


string stockphrases_hitboxes[2][3] = { {"(Damage Modifier) Hitbox","Calculated damage output on this hitbox is "," and so gets capped at 9999."},
u8"（ダメージ調整）部位",u8"この部位の計算上のダメージ出力は ",u8" なので、上限は 9999 です。" };



//Default language setting and the contents of the "dcff7config.txt" configuration file.
struct default_settings {
    int select_language()
    {

    languageloop:
        cout << " Set CMD font to 'MS Gothic' or other compatible font to display Japanese symbols." << endl;
        cout << u8" 日本語の記号を表示するには、CMD フォントを「MS ゴシック」またはその他の互換フォントに設定します。" << endl;
        cout << "\tSelect default language. Your choice is saved in [dcff7config.txt]." << endl;
        cout << u8"\tデフォルトの言語を選択。選択内容は「dcff7config.txt」に保存されます。" << endl;
        cout << u8"\t 0 = English/英語 \n\t 1 = 日本語/Japanese" << endl;
        cin >> language;
        if (language < 0 || language > 1)
        {
            cout << u8" Try again. リトライ。" << endl;
            goto languageloop;
        }

        ofstream configfile_write;                  //Writing the language setting to dcff7config.txt
        configfile_write.open("dcff7config.txt");
        configfile_write << language << u8"\n\n0 = English/英語\n1 = 日本語/Japanese\n";
        configfile_write.close();
        configfile_write.open("dcff7config.txt", ios::out | ios::app);
        configfile_write << u8"\nSet CMD font to 'MS Gothic' or other compatible font to display Japanese symbols in the program." << endl;
        configfile_write << u8"アプリケーションで日本語記号を表示するには、CMD フォントを「MS ゴシック」または別の互換フォントに設定します。" << endl;
        configfile_write << u8"\nYou can edit the number in the top left corner of this notepad to change the default language." << endl;
        configfile_write << u8"このメモ帳の左上隅にある数字を編集して、デフォルトの言語を変更できます。" << endl;

        configfile_write.close();

        return 0;
    }

    int controlconfigfile()
    {
        ifstream configfile_read;
        configfile_read.open("dcff7config.txt");
        if (configfile_read.is_open())         //If "dcff7config.txt" exists and is opened.
        {
            string line;
            getline(configfile_read, line);
            if (line[0] == (char)'0') //If "0 = English" then the default language is already set.
            {
                configfile_read.close();
                language = 0;
            }
            else if (line[0] == (char)'1') //If "1 = Japanese" then the default language is already set.
            {
                configfile_read.close();
                language = 1;
            }
            else
            {
                configfile_read.close();
                select_language();
            }
        }
        else   //If dcff7config does not exist, the file is created.
        {
            configfile_read.close();
            ofstream configfile_write;
            configfile_write.open("dcff7config.txt", ios::out | ios::app);
            configfile_write << u8"\n\n0 = English/英語\n1 = 日本語/Japanese\n";
            configfile_write << u8"\nSet CMD font to 'MS Gothic' or other compatible font to display Japanese symbols in the program." << endl;
            configfile_write << u8"アプリケーションで日本語記号を表示するには、CMD フォントを「MS ゴシック」または別の互換フォントに設定します。" << endl;
            configfile_write << u8"\nYou can edit the number in the top left corner of this notepad to change the default language." << endl;
            configfile_write << u8"このメモ帳の左上隅にある数字を編集して、デフォルトの言語を変更できます。" << endl;
            configfile_write.close();
            select_language();
        }
        return 0;
    }


    int language_error() {
        cout << u8" Language error || 言語エラー" << endl;
        cout << u8" Defaulting to English || デフォルトで英語" << endl;
        language = 0;
        return 0;
    }

};



struct calculations_postoriginal :public default_settings {


    string playerfile_save, playerfile_load, filename_full;

    int difficultymode_attackmodifier[2] = { 0,-50 };   //0 change to dmg output on Normal/Hard. -50 change to dmg output on Ex Hard.
    string difficultymodes[2][2] = { { "Normal/Hard","Ex Hard" },
                                   u8"NORMAL/HARD",u8"EX HARD" };

    int handgunpowervalues[16] = { 1,2,3,90,5,32,32,53,53,83,30,28,27,34,36,38 };
    string handguns[2][16] = { { "Model Gun","Silver Model Gun","Gold Model Gun","Ultima Weapon","Cerberus","Cerberus II","Cerberus III","P Cerberus","P Cerberus Gamma","Powered Cerberus","S Cerberus","S Cerberus Gamma","Zero Cerberus","M Cerberus","M Cerberus Gamma","Quantum Cerberus" },
u8"おもちゃの銃",u8"銀のおもちゃの銃",u8"金のおもちゃの銃",u8"アルテマウェポン",u8"ケルベロス",u8"ケルベロスⅡ",u8"ケルベロスⅢ",u8"Ｐケルベロス",u8"Ｐケルベロスγ",u8"パワードケルベロス",u8"Ｓケルベロス",u8"Ｓケルベロスγ",u8"ゼロケルベロス",u8"Ｍケルベロス",u8"Ｍケルベロスγ",u8"クォントケルベロス" };

    int machinegunpowervalues[15] = { 12,18,23,28,28,38,16,16,16,16,18,19,20,22,16 };
    string machineguns[2][15] = { { "Griffon","Griffon II","P Griffon","P Griffon Beta","P Griffon Gamma","Dueling Griffon","S Griffon","S Griffon Beta","S Griffon Gamma","Sonic Griffon","M Griffon","M Griffon Beta","M Griffon Gamma","Starry Griffon","Blast Machine Gun" },
u8"グリフォン",u8"グリフォンⅡ",u8"Ｐグリフォン",u8"Ｐグリフォンβ",u8"Ｐグリフォンγ",u8"デュエルグリフォン",u8"Ｓグリフォン",u8"Ｓグリフォンβ",u8"Ｓグリフォンγ",u8"ラピッドグリフォン",u8"Ｍグリフォン",u8"Ｍグリフォンβ",u8"Ｍグリフォンγ",u8"スターリーグリフォン",u8"ブロウマシンガン" };

    int riflepowervalues[13] = { 80,80,110,160,220,360,70,70,70,110,120,130,100 };
    string rifles[2][13] = { { "Hydra","Hydra II","Hydra III","P Hydra","P Hydra Gamma","Gigant Hydra","S Hydra","S Hydra Gamma","Feather Hydra","M Hydra","M Hydra Gamma","Velvet Hydra","Bayonet Rifle" },
u8"ヒドラ",u8"ヒドラⅡ",u8"ヒドラⅢ",u8"Ｐヒドラ",u8"Ｐヒドラγ",u8"ギガントヒドラ",u8"Ｓヒドラ",u8"Ｓヒドラγ",u8"フェザーヒドラ",u8"Ｍヒドラ",u8"Ｍヒドラγ",u8"ベルベットヒドラ",u8"バヨネットライフル" };

    int barrelpowervalues[21] = { 25,45,45,45,45,40,40,30,30,50,50,60,70,40,13,23,23,23,30,35,12 };
    string barrels[2][21] = { { "Normal Barrel","Normal Barrel II","Normal Barrel III","Normal Barrel Beta","Normal Barrel Gamma","Master N Barrel","Guard N Barrel","Long Barrel","Long Barrel II","Long Barrel III","Long Barrel Beta","Long Barrel Gamma","Nova L Barrel","Faerie L Barrel","Short Barrel","Short Barrel II","Short Barrel III","Short Barrel Beta","Short Barrel Gamma","Gale S Barrel","Lucky S Barrel" },
u8"ミドルバレル",u8"ミドルバレルⅡ",u8"ミドルバレルⅢ",u8"ミドルバレルβ",u8"ミドルバレルγ",u8"マスターＭバレル",u8"ガードＭバレル",u8"ロングバレル",u8"ロングバレルⅡ",u8"ロングバレルⅢ",u8"ロングバレルβ",u8"ロングバレルγ",u8"ノヴァＬバレル",u8"フェアリーＬバレル",u8"ショートバレル",u8"ショートバレルⅡ",u8"ショートバレルⅢ",u8"ショートバレルβ",u8"ショートバレルγ",u8"ゲイルＳバレル",u8"ラッキーＳバレル" };

    int powerboostervalues[4] = { 0,25,40,60 };
    string powerboosters[2][4] = { { "[No Power Booster]","Power Booster","Power Booster Beta","Power Booster Gamma" },
u8"［パワーブースターなし］",u8"パワーブースター",u8"パワーブースターβ",u8"パワーブースターγ" };

    int magicpowervalues[9] = { 200,300,400,250,600,999,450,600,750 };
    int magicmpconsumption[9] = { 14,17,20,17,25,33,16,20,23 };
    string magicspells[2][9] = { { "Fire Lv.1","Fire Lv.2","Fire Lv.3","Thunder Lv.1","Thunder Lv.2","Thunder Lv.3","Blizzard Lv.1","Blizzard Lv.2","Blizzard Lv.3" },
u8"ファイアLv.1",u8"ファイアLv.2",u8"ファイアLv.3",u8"サンダーLv.1",u8"サンダーLv.2",u8"サンダーLv.3",u8"ブリザドLv.1",u8"ブリザドLv.2",u8"ブリザドLv.3" };

    int manaboostervalues[3] = { 0,60,140 };
    string manaboosters[2][3] = { { "[No Mana Booster]","Manamind","Manasoul" },
u8"［魔法ブーストパーツなし］",u8"マインドオブメイジ",u8"ソウルオブメイジ" };
    //Manaheart does not boost magic power.


    //Function to select Difficulty Mode.
    int difficultymode_select()
    {

    selectingdifficulty:
        switch (language)
        {
        case 0: //English
            cout << "\nSelect the difficulty mode." << endl;
            cout << " 0 - Normal/Hard Mode:\tDamage output from player is normal." << endl;
            cout << " 1 - Ex Hard Mode:\tDamage output from player is slightly lower." << endl;
            cout << "Type the digit and press Enter: " << flush;
            break;
        case 1: //Japanese
            cout << u8"\n難易度を選択してください。" << endl;
            cout << u8" 0 - NORMAL/HARD：\t難易度による与ダメージ量の変化は発生しません。" << endl;
            cout << u8" 1 - EX HARD：\t\tプレイヤーから与えるダメージがわずかに減少します。" << endl;

            cout << u8"数字を入力し、Enter を押してください： " << flush;
            break;
        default:
            language_error();
        }

        cin >> difficultymode;
        if (difficultymode < 0 || difficultymode>1)
        {
            cout << stockphrases_error[language][0] << endl;
            goto selectingdifficulty;
        }
        cout << stockphrases_selection[language][1] << difficultymodes[language][difficultymode] << endl;
        return 0;
    }


    //Function for when selecting Vincent's Level.
    int vincentlv_select()
    {

    redovincentlvselect:
        switch (language)
        {
        case 0: //English
            cout << "\nType Vincent's exact Level or exact max HP and press enter." << endl;
            cout << "Valid Level range: 1 - 50 | Valid HP range: 780 - 3000" << endl;
            cout << "Value: ";
            break;
        case 1: //Japanese
            cout << u8"\nヴィンセントのレベルまたは最大 HP を正確に入力し、Enter キーを押してください。" << endl;
            cout << u8"有効レベル範囲：1 - 50 | 有効HP範囲：780 - 3000" << endl;
            cout << u8"入力数値： ";
            break;
        default:
            language_error();
        }

        cin >> vincentlevel;
        if (vincentlevel == 0)
        {
            cout << stockphrases_error[language][0] << endl;
            goto redovincentlvselect;
        }
        else if (vincentlevel > 0 && vincentlevel < 51)
        {
            goto vincentlevelselected;
        }
        else if (vincentlevel > 50)
        {
            //cycles through valid HP values. vincentlevel = vincentHP[X]
            for (int n = 1; n < 51; n++)
            {
                if (vincentlevel == vincentHP[n])
                {
                    vincentlevel = n;
                    n = 50;
                    goto vincentlevelselected;
                }
            }
            cout << stockphrases_error[language][0] << endl;
            goto  redovincentlvselect;

        }
        else
        {
            cout << stockphrases_error[language][0] << endl;
            goto  redovincentlvselect;
        }
    vincentlevelselected:

        if (statinfodump == 0)
        {
            switch (language)
            {
            case 0: //English
                cout << "Lv " << vincentlevel << flush;
                cout << " | Max HP " << vincentHP[vincentlevel] << flush;
                cout << " | STR " << vincentSTR[vincentlevel] << flush;
                cout << " | DEF " << vincentDEF[vincentlevel] << flush;
                cout << " | DEX " << vincentDEX[vincentlevel] << flush;
                cout << " | INT " << vincentINT[vincentlevel] << endl;
                cout << "\n********************ADDITIONAL INFO********************" << endl;
                cout << " STR slightly improves the power of physical attacks (bullets/melee)." << endl;
                cout << " DEF slightly improves physical defense." << endl;
                cout << " DEX slightly improves bullet-accuracy." << endl;
                cout << " INT slightly improves the power of magic attacks." << endl;
                cout << "*******************************************************" << endl;
                break;
            case 1: //Japanese
                cout << u8"レベル [Lv] " << vincentlevel << flush;
                cout << u8" | 最大HP " << vincentHP[vincentlevel] << flush;
                cout << u8" | STR " << vincentSTR[vincentlevel] << flush;
                cout << u8" | DEF " << vincentDEF[vincentlevel] << flush;
                cout << u8" | DEX " << vincentDEX[vincentlevel] << flush;
                cout << u8" | INT " << vincentINT[vincentlevel] << endl;
                cout << u8"\n======================追加情報======================" << endl;
                cout << u8" STRは、物理攻撃（射撃/格闘）の威力が少し増加します。" << endl;
                cout << u8" DEFは、物理防御力が少し増加します。" << endl;
                cout << u8" DEXは、銃撃の命中率が少し増加します。" << endl;
                cout << u8" INTは、魔法攻撃の威力が少し増加します。" << endl;
                cout << u8"====================================================" << endl;
                break;
            default:
                language_error();
            }
            statinfodump = 1;
        }

        return 0;
    }


    //Function for selecting how high your killchain should be and calculating the damage power.
    int chain_select()
    {

        switch (language)
        {
        case 0: //English
            cout << "\nHow high will your killchain be?" << endl;
            cout << "Valid value range: 0-99" << endl;
            break;
        case 1: //Japanese
            cout << u8"\nチェイン数を入力してください。" << endl;
            cout << u8"有効な数値範囲：0-99" << endl;
            break;
        default:
            language_error();
        }

    redochainselection:
        cout << stockphrases_selection[language][0] << flush;
        cin >> killchain;
        if (killchain < 0 || killchain > 99)
        {
            cout << stockphrases_error[language][0] << endl;
            goto redochainselection;
        }

        if (killchain<5 && killchain>-1) //if killchain is between 0 and 4
            chaindamage = killchain * 30;
        else if (killchain > 4 && killchain < 100) //if killchain is between 5 and 99
            chaindamage = (killchain * 100) - 300;
        switch (language)
        {
        case 0: //English
            cout << "You now have a " << killchain << "-chain." << endl;
            break;
        case 1: //Japanese
            cout << killchain << u8"チェインでダメージ計算を行います。" << endl;
            break;
        default:
            language_error();
        }
        return 0;
    }


    //Function for selecting a handgun.
    int handgun_select()
    {

        switch (language)
        {
        case 0: //English
            cout << "\nSelect a handgun by entering the corresponding number." << endl;
            cout << " 0  - Model Gun          ATK Power:  1" << endl;
            cout << " 1  - Silver Model Gun   ATK Power:  2" << endl;
            cout << " 2  - Gold Model Gun     ATK Power:  3" << endl;
            cout << " 3  - Ultima Weapon      ATK Power: 90" << endl;
            cout << " 4  - Cerberus           ATK Power:  5" << endl;
            cout << " 5  - Cerberus II        ATK Power: 32" << endl;
            cout << " 6  - Cerberus III       ATK Power: 32" << endl;
            cout << " 7  - P Cerberus         ATK Power: 53" << endl;
            cout << " 8  - P Cerberus Gamma   ATK Power: 53" << endl;
            cout << " 9  - Powered Cerberus   ATK Power: 83" << endl;
            cout << " 10 - S Cerberus         ATK Power: 30" << endl;
            cout << " 11 - S Cerberus Gamma   ATK Power: 28" << endl;
            cout << " 12 - Zero Cerberus      ATK Power: 27" << endl;
            cout << " 13 - M Cerberus         ATK Power: 34" << endl;
            cout << " 14 - M Cerberus Gamma   ATK Power: 36" << endl;
            cout << " 15 - Quantum Cerberus   ATK Power: 38" << endl;
            break;
        case 1: //Japanese
            cout << u8"\n対応する番号を入力して使用するハンドガンを選択してください。" << endl;
            cout << u8" 0  - おもちゃの銃\t\t攻撃力：  1" << endl;
            cout << u8" 1  - 銀のおもちゃの銃\t\t攻撃力：  2" << endl;
            cout << u8" 2  - 金のおもちゃの銃\t\t攻撃力：  3" << endl;
            cout << u8" 3  - アルテマウェポン\t\t攻撃力： 90" << endl;
            cout << u8" 4  - ケルベロス\t\t攻撃力：  5" << endl;
            cout << u8" 5  - ケルベロスⅡ\t\t攻撃力： 32" << endl;
            cout << u8" 6  - ケルベロスⅢ\t\t攻撃力： 32" << endl;
            cout << u8" 7  - Ｐケルベロス\t\t攻撃力： 53" << endl;
            cout << u8" 8  - Ｐケルベロスγ\t\t攻撃力： 53" << endl;
            cout << u8" 9  - パワードケルベロス\t攻撃力： 83" << endl;
            cout << u8" 10 - Ｓケルベロス\t\t攻撃力： 30" << endl;
            cout << u8" 11 - Ｓケルベロスγ\t\t攻撃力： 28" << endl;
            cout << u8" 12 - ゼロケルベロス\t\t攻撃力： 27" << endl;
            cout << u8" 13 - Ｍケルベロス\t\t攻撃力： 34" << endl;
            cout << u8" 14 - Ｍケルベロスγ\t\t攻撃力： 36" << endl;
            cout << u8" 15 - クォントケルベロス\t攻撃力： 38" << endl;
            break;
        default:
            language_error();
        }

    redohandgunselection:
        cout << stockphrases_selection[language][0] << flush;
        cin >> handgun;
        if (handgun < 0 || handgun > 15)
        {
            cout << stockphrases_error[language][0] << endl;
            goto redohandgunselection;
        }
        cout << stockphrases_selection[language][1] << handguns[language][handgun] << endl;
        framepower = handgunpowervalues[handgun];
        return 0;
    }


    //Function for selecting a machine gun.
    int machinegun_select()
    {
        switch (language)
        {
        case 0: //English
            cout << "\nSelect a machine gun by entering the corresponding number." << endl;
            cout << " 0  - Griffon            ATK Power: 12" << endl;
            cout << " 1  - Griffon II         ATK Power: 18" << endl;
            cout << " 2  - P Griffon          ATK Power: 23" << endl;
            cout << " 3  - P Griffon Beta     ATK Power: 28" << endl;
            cout << " 4  - P Griffon Gamma    ATK Power: 28" << endl;
            cout << " 5  - Dueling Griffon    ATK Power: 38" << endl;
            cout << " 6  - S Griffon          ATK Power: 16" << endl;
            cout << " 7  - S Griffon Beta     ATK Power: 16" << endl;
            cout << " 8  - S Griffon Gamma    ATK Power: 16" << endl;
            cout << " 9  - Sonic Griffon      ATK Power: 16" << endl;
            cout << " 10 - M Griffon          ATK Power: 18" << endl;
            cout << " 11 - M Griffon Beta     ATK Power: 19" << endl;
            cout << " 12 - M Griffon Gamma    ATK Power: 20" << endl;
            cout << " 13 - Starry Griffon     ATK Power: 22" << endl;
            cout << " 14 - Blast Machine Gun  ATK Power: 16" << endl;
            break;
        case 1: //Japanese
            cout << u8"\n対応する番号を入力して使用するマシンガンを選択してください。" << endl;
            cout << u8" 0  - グリフォン\t\t攻撃力： 12" << endl;
            cout << u8" 1  - グリフォンⅡ\t\t攻撃力： 18" << endl;
            cout << u8" 2  - Ｐグリフォン\t\t攻撃力： 23" << endl;
            cout << u8" 3  - Ｐグリフォンβ\t\t攻撃力： 28" << endl;
            cout << u8" 4  - Ｐグリフォンγ\t\t攻撃力： 28" << endl;
            cout << u8" 5  - デュエルグリフォン\t攻撃力： 38" << endl;
            cout << u8" 6  - Ｓグリフォン\t\t攻撃力： 16" << endl;
            cout << u8" 7  - Ｓグリフォンβ\t\t攻撃力： 16" << endl;
            cout << u8" 8  - Ｓグリフォンγ\t\t攻撃力： 16" << endl;
            cout << u8" 9  - ラピッドグリフォン\t攻撃力： 16" << endl;
            cout << u8" 10 - Ｍグリフォン\t\t攻撃力： 18" << endl;
            cout << u8" 11 - Ｍグリフォンβ\t\t攻撃力： 19" << endl;
            cout << u8" 12 - Ｍグリフォンγ\t\t攻撃力： 20" << endl;
            cout << u8" 13 - スターリーグリフォン\t攻撃力： 22" << endl;
            cout << u8" 14 - ブロウマシンガン\t\t攻撃力： 16" << endl;
            break;
        default:
            language_error();
        }

    redomachinegunselection:
        cout << stockphrases_selection[language][0] << flush;
        cin >> machinegun;
        if (machinegun < 0 || machinegun > 14)
        {
            cout << stockphrases_error[language][0] << endl;
            goto redomachinegunselection;
        }
        cout << stockphrases_selection[language][1] << machineguns[language][machinegun] << endl;
        framepower = machinegunpowervalues[machinegun];
        return 0;
    }


    //Function for selecting a rifle.
    int rifle_select()
    {
        switch (language)
        {
        case 0: //English
            cout << "\nSelect a rifle by entering the corresponding number." << endl;
            cout << " 0  - Hydra            ATK Power:  80" << endl;
            cout << " 1  - Hydra II         ATK Power:  80" << endl;
            cout << " 2  - Hydra III        ATK Power: 110" << endl;
            cout << " 3  - P Hydra          ATK Power: 160" << endl;
            cout << " 4  - P Hydra Gamma    ATK Power: 220" << endl;
            cout << " 5  - Gigant Hydra     ATK Power: 360" << endl;
            cout << " 6  - S Hydra          ATK Power:  70" << endl;
            cout << " 7  - S Hydra Gamma    ATK Power:  70" << endl;
            cout << " 8  - Feather Hydra    ATK Power:  70" << endl;
            cout << " 9  - M Hydra          ATK Power: 110" << endl;
            cout << " 10 - M Hydra Gamma    ATK Power: 120" << endl;
            cout << " 11 - Velvet Hydra     ATK Power: 130" << endl;
            cout << " 12 - Bayonet Rifle    ATK Power: 100" << endl;
            break;
        case 1: //Japanese
            cout << u8"対応する番号を入力して使用するライフルを選択してください。" << endl;
            cout << u8" 0  - ヒドラ\t\t\t攻撃力：  80" << endl;
            cout << u8" 1  - ヒドラⅡ\t\t\t攻撃力：  80" << endl;
            cout << u8" 2  - ヒドラⅢ\t\t\t攻撃力： 110" << endl;
            cout << u8" 3  - Ｐヒドラ\t\t\t攻撃力： 160" << endl;
            cout << u8" 4  - Ｐヒドラγ\t\t攻撃力： 220" << endl;
            cout << u8" 5  - ギガントヒドラ\t\t攻撃力： 360" << endl;
            cout << u8" 6  - Ｓヒドラ\t\t\t攻撃力：  70" << endl;
            cout << u8" 7  - Ｓヒドラγ\t\t攻撃力：  70" << endl;
            cout << u8" 8  - フェザーヒドラ\t\t攻撃力：  70" << endl;
            cout << u8" 9  - Ｍヒドラ\t\t\t攻撃力： 110" << endl;
            cout << u8" 10 - Ｍヒドラγ\t\t攻撃力： 120" << endl;
            cout << u8" 11 - ベルベットヒドラ\t\t攻撃力： 130" << endl;
            cout << u8" 12 - バヨネットライフル\t攻撃力： 100" << endl;
            break;
        default:
            language_error();
        }

    redorifleselection:
        cout << stockphrases_selection[language][0] << flush;
        cin >> rifle;
        if (rifle < 0 || rifle > 12)
        {
            cout << stockphrases_error[language][0] << endl;
            goto redorifleselection;
        }
        cout << stockphrases_selection[language][1] << rifles[language][rifle] << endl;
        framepower = riflepowervalues[rifle];
        return 0;
    }


    //Function for selecting a barrel.
    int barrel_select()
    {
        switch (language)
        {
        case 0: //English
            cout << "\nSelect a barrel by entering the corresponding number." << endl;
            cout << " 0  - Normal Barrel        ATK Power: 25" << endl;
            cout << " 1  - Normal Barrel II     ATK Power: 45" << endl;
            cout << " 2  - Normal Barrel III    ATK Power: 45" << endl;
            cout << " 3  - Normal Barrel Beta   ATK Power: 45" << endl;
            cout << " 4  - Normal Barrel Gamma  ATK Power: 45" << endl;
            cout << " 5  - Master N Barrel      ATK Power: 40" << endl;
            cout << " 6  - Guard N Barrel       ATK Power: 40" << endl;
            cout << " 7  - Long Barrel          ATK Power: 30" << endl;
            cout << " 8  - Long Barrel II       ATK Power: 30" << endl;
            cout << " 9  - Long Barrel III      ATK Power: 50" << endl;
            cout << " 10 - Long Barrel Beta     ATK Power: 50" << endl;
            cout << " 11 - Long Barrel Gamma    ATK Power: 60" << endl;
            cout << " 12 - Nova L Barrel        ATK Power: 70" << endl;
            cout << " 13 - Faerie L Barrel      ATK Power: 40" << endl;
            cout << " 14 - Short Barrel         ATK Power: 13" << endl;
            cout << " 15 - Short Barrel II      ATK Power: 23" << endl;
            cout << " 16 - Short Barrel III     ATK Power: 23" << endl;
            cout << " 17 - Short Barrel Beta    ATK Power: 23" << endl;
            cout << " 18 - Short Barrel Gamma   ATK Power: 30" << endl;
            cout << " 19 - Gale S Barrel        ATK Power: 35" << endl;
            cout << " 20 - Lucky S Barrel       ATK Power: 12" << endl;
            break;
        case 1: //Japanese
            cout << u8"対応する番号を入力して使用するバレルを選択してください。" << endl;
            cout << u8" 0  - ミドルバレル\t\t攻撃力： 25" << endl;
            cout << u8" 1  - ミドルバレルⅡ\t\t攻撃力： 45" << endl;
            cout << u8" 2  - ミドルバレルⅢ\t\t攻撃力： 45" << endl;
            cout << u8" 3  - ミドルバレルβ\t\t攻撃力： 45" << endl;
            cout << u8" 4  - ミドルバレルγ\t\t攻撃力： 45" << endl;
            cout << u8" 5  - マスターＭバレル\t\t攻撃力： 40" << endl;
            cout << u8" 6  - ガードＭバレル\t\t攻撃力： 40" << endl;
            cout << u8" 7  - ロングバレル\t\t攻撃力： 30" << endl;
            cout << u8" 8  - ロングバレルⅡ\t\t攻撃力： 30" << endl;
            cout << u8" 9  - ロングバレルⅢ\t\t攻撃力： 50" << endl;
            cout << u8" 10 - ロングバレルβ\t\t攻撃力： 50" << endl;
            cout << u8" 11 - ロングバレルγ\t\t攻撃力： 60" << endl;
            cout << u8" 12 - ノヴァＬバレル\t\t攻撃力： 70" << endl;
            cout << u8" 13 - フェアリーＬバレル\t攻撃力： 40" << endl;
            cout << u8" 14 - ショートバレル\t\t攻撃力： 13" << endl;
            cout << u8" 15 - ショートバレルⅡ\t\t攻撃力： 23" << endl;
            cout << u8" 16 - ショートバレルⅢ\t\t攻撃力： 23" << endl;
            cout << u8" 17 - ショートバレルβ\t\t攻撃力： 23" << endl;
            cout << u8" 18 - ショートバレルγ\t\t攻撃力： 30" << endl;
            cout << u8" 19 - ゲイルＳバレル\t\t攻撃力： 35" << endl;
            cout << u8" 20 - ラッキーＳバレル\t\t攻撃力： 12" << endl;
            break;
        default:
            language_error();
        }

    redobarrelselection:
        cout << stockphrases_selection[language][0] << flush;
        cin >> barrel;
        if (barrel < 0 || barrel > 20)
        {
            cout << stockphrases_error[language][0] << endl;
            goto redobarrelselection;
        }
        cout << stockphrases_selection[language][1] << barrels[language][barrel] << endl;
        barrelpower = barrelpowervalues[barrel];
        return 0;
    }


    //Function for selecting a Power Booster or choosing to equip none.
    int powerbooster_select()
    {

        switch (language)
        {
        case 0: //English
            cout << "\nATK Power of Frame + Barrel = " << framepower + barrelpower << endl;
            cout << "\nPower Booster selection." << endl;
            cout << " 0 - No Power Booster    ( 0% power increase)" << flush;
            cout << " ATK Power: " << framepower + barrelpower << endl;
            cout << " 1 - Power Booster       (25% power increase)" << flush;
            totalatkpower = (framepower + barrelpower) * (powerboostervalues[1] + 100) * dividebyonehundred.zeropointzeroone;
            cout << " New ATK Power: " << totalatkpower << endl;
            cout << " 2 - Power Booster Beta  (40% power increase)" << flush;
            totalatkpower = (framepower + barrelpower) * (powerboostervalues[2] + 100) * dividebyonehundred.zeropointzeroone;
            cout << " New ATK Power: " << totalatkpower << endl;
            cout << " 3 - Power Booster Gamma (60% power increase)" << flush;
            totalatkpower = (framepower + barrelpower) * (powerboostervalues[3] + 100) * dividebyonehundred.zeropointzeroone;
            cout << " New ATK Power: " << totalatkpower << endl;
            break;
        case 1: //Japanese
            cout << u8"\nフレームの攻撃力 + バレル = " << framepower + barrelpower << endl;
            cout << u8"\nパワーブースターの有無を設定してください。" << endl;
            cout << u8" 0 - パワーブースターなし（0% パワーアップ）" << flush;
            cout << u8" 攻撃力： " << framepower + barrelpower << endl;
            cout << u8" 1 - パワーブースター\t（25% パワーアップ）" << flush;
            totalatkpower = (framepower + barrelpower) * (powerboostervalues[1] + 100) * dividebyonehundred.zeropointzeroone;
            cout << u8" 計算後の攻撃力： " << totalatkpower << endl;
            cout << u8" 2 - パワーブースターβ\t（40% パワーアップ）" << flush;
            totalatkpower = (framepower + barrelpower) * (powerboostervalues[2] + 100) * dividebyonehundred.zeropointzeroone;
            cout << u8" 計算後の攻撃力： " << totalatkpower << endl;
            cout << u8" 3 - パワーブースターγ\t（60% パワーアップ）" << flush;
            totalatkpower = (framepower + barrelpower) * (powerboostervalues[3] + 100) * dividebyonehundred.zeropointzeroone;
            cout << u8" 計算後の攻撃力： " << totalatkpower << endl;
            break;
        default:
            language_error();
        }

    redopowerboosterselection:
        cout << stockphrases_selection[language][0] << flush;
        cin >> powerbooster;
        if (powerbooster < 0 || powerbooster > 3)
        {
            cout << stockphrases_error[language][0] << endl;
            goto redopowerboosterselection;
        }
        cout << stockphrases_selection[language][1] << powerboosters[language][powerbooster] << endl;
        return 0;
    }


    //Function for selecting boss to apply bullet damage on.
    int selectboss_bulletdmg() //Damage type is 0, 1 or 2.
    {
        //Every enemy with a visible HP bar.
        switch (language)
        {
        case 0: //English
            cout << "\nCurrently available selections:" << endl;
            cout << " 0  - Dragonfly              (Ch1-3)   N: HP 1600 | H/ExH: HP  2800" << endl;      //Sea of Flames
            cout << " 1  - Dragonfly GL           (Ch1-6)   N: HP 7000 | H/ExH: HP 12000" << endl;      //Sea of Flames
            cout << " 2  - Crimson Hound          (Ch2-4)   N: HP 1500 | H/ExH: HP  2500" << endl;      //Showdown in the Wastes
            cout << " 3  - Pegasus Riders         (Ch2-5)   N: HP  300(*5) | H/ExH: 1200(*5)" << endl;  //Showdown in the Wastes
            cout << " 4  - Heavy Armored Soldier  (Ch3-4)   HP   4800" << endl;                         //Silent Edge
            cout << " 5  - Shelke the Transparent (Ch4-3)   HP   3000" << endl;                         //Headquarters Under Siege
            cout << " 6  - Azul the Cerulean      (Ch4-4)   HP   4150" << endl;                         //Headquarters Under Siege
            cout << " 7  - Black Widow            (Ch5-4)   HP  10000" << endl;                         //Manor of Despair
            cout << " 8  - Black Widow II         (Ch6-3)   HP   4500" << endl;                         //Deepground Strikes Back
            cout << " 9  - Deepground Commander   (Ch8-1-2) HP   5400" << endl;                         //Midgar Assault
            cout << " 10 - Shrikes                (Ch8-1-3) HP    550(*6)" << endl;                     //Midgar Assault
            cout << " 11 - Rosso the Crimson      (Ch8-2-3) HP  17000" << endl;                         //Fight for the Central Complex
            cout << " 12 - Black Widow TW         (Ch9-3)   HP   2000(*2)" << endl;                     //An Empire in Ruins
            cout << " 13 - Neo Azul               (Ch9-5)   HP   5000" << endl;                         //An Empire in Ruins
            cout << " 14 - Arch Azul              (Ch9-5)   HP  13000" << endl;                         //An Empire in Ruins
            cout << " 15 - Nero the Sable         (Ch10-5)  HP  14500" << endl;                         //Shinra's Dark Secret
            cout << " 16 - Dragonfly PT           (Ch11-3)  HP  15000" << endl;                         //Beginnings
            cout << " 17 - Arachnero              (Ch11-4)  HP  12000" << endl;                         //Beginnings
            cout << " 18 - Gorgonero              (Ch11-4)  HP   3000" << endl;                         //Beginnings
            cout << " 19 - Weiss the Immaculate   (Ch11-5)  HP  30000" << endl;                         //Beginnings
            cout << " 20 - Weiss Empowered        (Ch11-6)  HP  16000" << endl;                         //Beginnings
            cout << " 21 - Crystal Feelers        (Ch12-2)  HP      0(*6)" << endl;                     //A Finale Chaotic
            cout << " 22 - Omega Cocoon           (Ch12-2)  HP  15001" << endl;                         //A Finale Chaotic
            cout << " 23 - Omega Weiss            (Ch12-2)  HP 240000" << endl;                         //A Finale Chaotic
            break;
        case 1: //Japanese
            cout << u8"\n現在利用可能な選択肢：" << endl;
            cout << u8" 0  - ドラゴンフライヤー\t\t(1章-3節)\tN: HP 1600 | H/ExH: HP  2800" << endl;      //Sea of Flames
            cout << u8" 1  - ドラゴンフライヤーＧＬ\t\t(1章-6節)\tN: HP 7000 | H/ExH: HP 12000" << endl;  //Sea of Flames
            cout << u8" 2  - クリムゾンハウンド\t\t(2章-4節)\tN: HP 1500 | H/ExH: HP  2500" << endl;     //Showdown in the Wastes
            cout << u8" 3  - エアホース隊\t\t\t(2章-5節)\tN: HP  300(*5) | H/ExH: 1200(*5)" << endl;    //Showdown in the Wastes
            cout << u8" 4  - ヘビーマウントソルジャー\t\t(3章-4節)\tHP   4800" << endl;                   //Silent Edge
            cout << u8" 5  - 無式のシェルク\t\t\t(4章-3節)\tHP   3000" << endl;                         //Headquarters Under Siege
            cout << u8" 6  - 蒼きアスール\t\t\t(4章-4節)\tHP   4150" << endl;                           //Headquarters Under Siege
            cout << u8" 7  - ブラック・ウィドー\t\t(5章-4節)\tHP  10000" << endl;                       //Manor of Despair
            cout << u8" 8  - ブラック・ウィドーⅡ\t\t(6章-3節)\tHP   4500" << endl;                        //Deepground Strikes Back
            cout << u8" 9  - ＤＧＳ　トルーパー１ｓｔ\t\t(8章-1節-2話)\tHP   5400" << endl;               //Midgar Assault
            cout << u8" 10 - シュライク部隊\t\t\t(8章-1節-3話)\tHP    550(*6)" << endl;                  //Midgar Assault
            cout << u8" 11 - 朱のロッソ＆朱のロッソ　バースト\t(8章-2節-3話)\tHP  17000" << endl;           //Fight for the Central Complex
            cout << u8" 12 - ブラック・ウィドーＴＷ\t\t(9章-3節)\tHP   2000(*2)" << endl;                 //An Empire in Ruins
            cout << u8" 13 - 蒼きアスール　イークイップ２\t(9章-5節)\tHP   5000" << endl;                  //An Empire in Ruins
            cout << u8" 14 - 真・アスール\t\t\t(9章-5節)\tHP  13000" << endl;                           //An Empire in Ruins
            cout << u8" 15 - 漆黒の闇ネロ\t\t\t(10章-5節)\tHP  14500" << endl;                          //Shinra's Dark Secret
            cout << u8" 16 - ドラゴンフライヤーＰＴ\t\t(11章-3節)\tHP  15000" << endl;                    //Beginnings
            cout << u8" 17 - ネロ・ラフレア\t\t\t(11章-4節)\tHP  12000" << endl;                         //Beginnings
            cout << u8" 18 - ネロ・ウィングシューター\t\t(11章-4節)\tHP   3000" << endl;                  //Beginnings
            cout << u8" 19 - 純白の帝王ヴァイス\t\t(11章-5節)\tHP  30000" << endl;                        //Beginnings
            cout << u8" 20 - オメガの力宿りしヴァイス\t\t(11章-6節)\tHP  16000" << endl;                   //Beginnings
            cout << u8" 21 - クリスタルフィーラー\t\t(12章-2節)\tHP      0(*6)" << endl;                  //A Finale Chaotic
            cout << u8" 22 - オメガコクーン\t\t\t(12章-2節)\tHP  15001" << endl;                         //A Finale Chaotic
            cout << u8" 23 - オメガヴァイス\t\t\t(12章-2節)\tHP 240000" << endl;                         //A Finale Chaotic
            break;
        default:
            language_error();
        }

    labelselectbossforbulletdamage:
        cout << stockphrases_selection[language][0] << flush;
        cin >> selectedboss;
        if (selectedboss < 0 || selectedboss > 23)
        {
            cout << stockphrases_error[language][0] << endl;
            goto labelselectbossforbulletdamage;
        }
        bossinfodump = 0;
        return 0;
    }


    //Function for selecting mana booster.
    int manabooster_select()
    {

        switch (language)
        {
        case 0: //English
            cout << "\nSelect a mana booster by entering the corresponding number." << endl;
            if (manainfodump == 0)
            {
                cout << "\n********************ADDITIONAL INFO********************" << endl;
                cout << "Mana boosters are only available in Ex Hard mode." << endl;
                cout << "The item Manaheart does not boost magic power." << endl;
                cout << "*******************************************************\n" << endl;
                manainfodump = 1;
            }
            cout << " 0 - No Mana Booster(  0% magic power increase)" << endl;
            cout << " 1 - Manamind       ( 60% magic power increase)" << endl;
            cout << " 2 - Manasoul       (140% magic power increase)" << endl;
            break;
        case 1: //Japanese
            cout << u8"\n対応する番号を入力して魔法ブーストパーツを選択してください。" << endl;
            if (manainfodump == 0)
            {
                cout << u8"\n======================追加情報======================" << endl;
                cout << u8"魔法ブーストパーツはEX HARDモードでのみ使用できます。" << endl;
                cout << u8"「ハートオブメイジ」は魔法威力が増加しません。" << endl;
                cout << u8"====================================================\n" << endl;
                manainfodump = 1;
            }
            cout << u8" 0 - 魔法ブーストパーツなし\t（魔法威力0%アップ）" << endl;
            cout << u8" 1 - マインドオブメイジ\t\t（魔法威力60%アップ）" << endl;
            cout << u8" 2 - ソウルオブメイジ\t\t（魔法威力140%アップ）" << endl;
            break;
        default:
            language_error();
        }

    redomanaboosterselect:
        cout << stockphrases_selection[language][0] << flush;
        cin >> manabooster;
        if (manabooster < 0 || manabooster > 2)
        {
            cout << stockphrases_error[language][0] << endl;
            goto redomanaboosterselect;
        }
        cout << stockphrases_selection[language][1] << manaboosters[language][manabooster] << endl;
        return 0;
    }


    //Select if enemy's magic defense should be normal, double or quadruple.
    int magicdefense_select()
    {
        switch (language)
        {
        case 0: //English
            cout << "\nSelect a Magic Defense setting for the enemy." << endl;
            if (enemymagicdefenseinfodump == 0)

            {
                cout << "\n********************ADDITIONAL INFO********************" << endl;
                cout << "  -All bosses with Double Magic Defense-" << endl;
                cout << "    Rosso the Crimson (Ch8-2-3)" << endl;
                cout << "    Nero the Sable    (Ch10-5)" << endl;
                cout << "    Dragonfly PT      (Ch11-3)" << endl;
                cout << "    Gorgonero         (Ch11-4)" << endl;
                cout << "    Omega Cocoon      (Ch12-2)" << endl;

                cout << "\n  -All bosses with Quadruple Magic Defense-" << endl;
                cout << "    Weiss the Immaculate (Ch11-5)" << endl;
                cout << "    Weiss Empowered      (Ch11-6)" << endl;
                cout << "*******************************************************\n" << endl;
                enemymagicdefenseinfodump = 1;
            }
            cout << " 0 - Normal Damage" << endl;
            cout << " 1 - Magic damage reduced to 1/2" << endl;
            cout << " 2 - Magic damage reduced to 1/4" << endl;
            break;
        case 1: //Japanese
            cout << u8"\n敵の魔法防御設定を選択してください。" << endl;
            if (enemymagicdefenseinfodump == 0)

            {
                cout << u8"\n======================追加情報======================" << endl;
                cout << u8"  -魔法防御 2倍のボス一覧-" << endl;
                cout << u8"    朱のロッソ\t\t\t(8章-2節-3話)" << endl;
                cout << u8"    漆黒の闇ネロ\t\t(10章-5節)" << endl;
                cout << u8"    ドラゴンフライヤーＰＴ\t(11章-3節)" << endl;
                cout << u8"    ネロ・ウィングシューター\t(11章-4節)" << endl;
                cout << u8"    オメガコクーン\t\t(12章-2節)" << endl;

                cout << u8"\n  -魔法防御 4倍のボス一覧-" << endl;
                cout << u8"    純白の帝王ヴァイス\t\t(11章-5節)" << endl;
                cout << u8"    オメガの力宿りしヴァイス\t(11章-6節)" << endl;
                cout << u8"====================================================\n" << endl;
                enemymagicdefenseinfodump = 1;
            }
            cout << u8" 0 - 魔法ダメージの軽減なし" << endl;
            cout << u8" 1 - 魔法ダメージを1/2軽減。" << endl;
            cout << u8" 2 - 魔法ダメージを1/4軽減。" << endl;
            break;
        default:
            language_error();
        }


    redoenemymagicdefselection:
        cout << stockphrases_selection[language][0] << flush;
        cin >> magicdamagelimiter;
        if (magicdamagelimiter < 0 || magicdamagelimiter > 2)
        {
            cout << stockphrases_error[language][0] << endl;
            goto redoenemymagicdefselection;
        }
        cout << stockphrases_selection[language][1] << enemymagicdefense[language][magicdamagelimiter] << endl;
        return 0;

    }


    int magicdmg() {

        baseINTpower = (vincentINT[vincentlevel] * magiccoefficient.magicfloat) + magicconstant.magicfloat + 0.50;

        switch (language)
        {
        case 0: //English
            cout << "\n-Calculated damage output of all elemental magic-" << endl;
            break;
        case 1: //Japanese
            cout << u8"\n-すべての属性魔法のダメージ出力計算値-" << endl;
            break;
        default:
            language_error();
        }

        for (magic = 0; magic < 9; magic++) //Iterates every magic spell.
        {
            cout << " " << magicspells[language][magic] << "\t: " << flush; //Prints out name of the magic and its level.
            if (manabooster == 0)
            {
                totalspellpower = magicpowervalues[magic] + 0.50;
            } //^Documentation lost: Not sure if the game itself skips the 100 * 0.00999999977648 here.
            else
            {
                totalspellpower = (magicpowervalues[magic] * dividebyonehundred.zeropointzeroone * (100 + manaboostervalues[manabooster])) + 0.50;
            }
            //enemyDEF stat does not influence power of the player's magic attacks.
            preliminarydamage = (totalspellpower + 0.50) * (baseINTpower + difficultymode_attackmodifier[difficultymode] + chaindamage + 100) * dividebyonehundred.zeropointzeroone;
            if (magicdamagelimiter == 0)
            {
                preliminarydamage = (preliminarydamage * 1.00) + 0.50;
            }
            else if (magicdamagelimiter == 1)
            {
                preliminarydamage = (preliminarydamage * 0.50) + 0.50;
            }
            else if (magicdamagelimiter == 2)
            {
                preliminarydamage = (preliminarydamage * 0.25) + 0.50;
            }
            cout << preliminarydamage << flush;

            if ((magic == 0 || magic == 1 || magic == 2 || magic == 6 || magic == 7 || magic == 8) && preliminarydamage > 9999) //If Fire or Blizzard magic.
                cout << stockphrases_damagecaps[language][0] << flush;
            else if ((magic == 3 || magic == 4 || magic == 5) && preliminarydamage > 9999 && magicdamagelimiter == 0) //If Thunder magic and normal damage cap of 9999.
                cout << stockphrases_damagecaps[language][0] << flush;
            else if ((magic == 3 || magic == 4 || magic == 5) && preliminarydamage > 5000 && magicdamagelimiter == 1) //If Thunder magic then damage caps out at 5000 here.
                cout << stockphrases_damagecaps[language][1] << flush;
            else if ((magic == 3 || magic == 4 || magic == 5) && preliminarydamage > 2500 && magicdamagelimiter == 2) //If Thunder magic then damage caps out at 2500 here.
                cout << stockphrases_damagecaps[language][2] << flush;

            cout << "" << endl;
        }
        return 0;
    }


    int magicandmp_info()
    {
        //Info about MP consumption, restoratives, Mako Points etc
        switch (language)
        {
        case 0: //English
            cout << "\n********************MAGIC- & MP INFO********************" << endl;
            cout << " Power Boosters do not increase damage output from magic." << endl;
            cout << " Only the two 'Mana Booster' items Manamind and Manasoul do that.\n" << endl;

            cout << " Fire Lv.1      MAG Power: 200  [MP Consumption: 14]" << endl;
            cout << " Fire Lv.2      MAG Power: 300  [MP Consumption: 17]" << endl;
            cout << " Fire Lv.3      MAG Power: 400  [MP Consumption: 20]" << endl;
            cout << " Thunder Lv.1   MAG Power: 250  [MP Consumption: 17]" << endl;
            cout << " Thunder Lv.2   MAG Power: 600  [MP Consumption: 25]" << endl;
            cout << " Thunder Lv.3   MAG Power: 999  [MP Consumption: 33]" << endl;
            cout << " Blizzard Lv.1  MAG Power: 450  [MP Consumption: 16]" << endl;
            cout << " Blizzard Lv.2  MAG Power: 600  [MP Consumption: 20]" << endl;
            cout << " Blizzard Lv.3  MAG Power: 750  [MP Consumption: 23]" << endl;

            cout << "\n MP restorative - MP restoration amount" << endl;
            cout << " Mako Point     - 34" << endl;
            cout << " Spirit Rations - 20" << endl;
            cout << " Ether          - 75" << endl;
            cout << " Red Ether      - 5, 25, 50, 70 or all MP" << endl;
            cout << " Elixir         - All HP & MP" << endl;
            cout << " Elixor         - All HP, MP & grants auto-life status" << endl;

            cout << "\n Vincent's max MP is 100." << endl;
            cout << "*************************************************************" << endl;
            break;
        case 1: //Japanese
            cout << u8"\n========================魔力・MP情報========================" << endl;
            cout << u8" パワーブースターは魔法威力は増加するものではありません。" << endl;
            cout << u8" 魔法ブーストパーツ「マインドオブメイジ」「ソウルオブメイジ」" << endl;
            cout << u8" でのみ魔法威力の増加が可能です。\n" << endl;

            cout << u8" ファイアLv.1\t魔力： 200 ［消費MP： 14］" << endl;
            cout << u8" ファイアLv.2\t魔力： 300 ［消費MP： 17］" << endl;
            cout << u8" ファイアLv.3\t魔力： 400 ［消費MP： 20］" << endl;
            cout << u8" サンダーLv.1\t魔力： 250 ［消費MP： 17］" << endl;
            cout << u8" サンダーLv.2\t魔力： 600 ［消費MP： 25］" << endl;
            cout << u8" サンダーLv.3\t魔力： 999 ［消費MP： 33］" << endl;
            cout << u8" ブリザドLv.1\t魔力： 450 ［消費MP： 16］" << endl;
            cout << u8" ブリザドLv.2\t魔力： 600 ［消費MP： 20］" << endl;
            cout << u8" ブリザドLv.3\t魔力： 750 ［消費MP： 23］" << endl;

            cout << u8"\n MP回復\t\t- MP回復量" << endl;
            cout << u8" 魔晄ポイント\t- 34" << endl;
            cout << u8" 精神回復薬\t- 20" << endl;
            cout << u8" エーテル\t- 75" << endl;
            cout << u8" レッドエーテル\t- 5、25、50、70 またはすべての MP" << endl;
            cout << u8" エリクサー\t- HPとMPを全回復" << endl;
            cout << u8" エリク茶\t- HP・MP全回復＆リレイズ状態付与" << endl;

            cout << u8"\n ヴィンセントの最大MPは100。" << endl;
            cout << u8"============================================================" << endl;
            break;
        default:
            language_error();
        }
        return 0;
    }


    int boss_info() {

        switch (language)
        {
        case 0: //English
            cout << "\n************************BOSS INFO************************" << endl;

            cout << " Dragonfly              (Ch1-3)   N: HP 1600 | H/ExH: HP  2800" << endl;      //Sea of Flames
            cout << " Dragonfly GL           (Ch1-6)   N: HP 7000 | H/ExH: HP 12000" << endl;      //Sea of Flames
            cout << " Crimson Hound          (Ch2-4)   N: HP 1500 | H/ExH: HP  2500" << endl;      //Showdown in the Wastes
            cout << " Pegasus Riders         (Ch2-5)   N: HP  300(*5) | H/ExH: 1200(*5)" << endl;  //Showdown in the Wastes
            cout << " Heavy Armored Soldier  (Ch3-4)   HP   4800" << endl;                         //Silent Edge
            cout << " Shelke the Transparent (Ch4-3)   HP   3000" << endl;                         //Headquarters Under Siege
            cout << " Azul the Cerulean      (Ch4-4)   HP   4150 | Immune to magic" << endl;       //Headquarters Under Siege
            cout << " Black Widow            (Ch5-4)   HP  10000" << endl;                         //Manor of Despair
            cout << " Black Widow II         (Ch6-3)   HP   4500" << endl;                         //Deepground Strikes Back
            cout << " Deepground Commander   (Ch8-1-2) HP   5400" << endl;                         //Midgar Assault
            cout << " Shrikes                (Ch8-1-3) HP    550(*6)" << endl;                     //Midgar Assault
            cout << " Rosso the Crimson      (Ch8-2-3) HP  17000 | 2x Magic Defense" << endl;      //Fight for the Central Complex
            cout << " Black Widow TW         (Ch9-3)   HP   2000(*2)" << endl;                     //An Empire in Ruins
            cout << " Neo Azul               (Ch9-5)   HP   5000" << endl;                         //An Empire in Ruins
            cout << " Arch Azul              (Ch9-5)   HP  13000" << endl;                         //An Empire in Ruins
            cout << " Nero the Sable         (Ch10-5)  HP   4000 + 2000*2 + 6500 | 2x Magic Defense" << endl;  //Shinra's Dark Secret
            cout << " Dragonfly PT           (Ch11-3)  HP  15000 | 2x Magic Defense" << endl;                 //Beginnings
            cout << " Arachnero              (Ch11-4)  HP  12000" << endl;                         //Beginnings
            cout << " Arachnero's Shield     (Ch11-4)  HP   3500" << endl;
            cout << " Phalanxphere           (Ch11-4)  HP   1000" << endl;
            cout << " Gorgonero              (Ch11-4)  HP   3000 | 2x Magic Defense" << endl;      //Beginnings
            cout << " Weiss the Immaculate   (Ch11-5)  HP  30000 | 4x Magic Defense" << endl;      //Beginnings
            cout << " Weiss Empowered        (Ch11-6)  HP  16000 | 4x Magic Defense" << endl;      //Beginnings
            cout << " Omega Cocoon           (Ch12-2)  HP  15001 | 2x Magic Defense" << endl;      //A Finale Chaotic
            cout << " Omega Weiss            (Ch12-2)  HP 240000" << endl;                         //A Finale Chaotic

            cout << "\n Thunder damage is capped at 5000 for targets with double magic defense." << endl;
            cout << " Thunder damage is capped at 2500 for targets with quadruple magic defense." << endl;

            cout << "*********************************************************" << endl;
            break;
        case 1: //Japanese
            cout << u8"\n==============================ボス情報==============================" << endl;

            cout << u8" ドラゴンフライヤー\t\t\t(1章-3節)\tN: HP 1600 | H/ExH: HP  2800" << endl;      //Sea of Flames
            cout << u8" ドラゴンフライヤーＧＬ\t\t\t(1章-6節)\tN: HP 7000 | H/ExH: HP 12000" << endl;  //Sea of Flames
            cout << u8" クリムゾンハウンド\t\t\t(2章-4節)\tN: HP 1500 | H/ExH: HP  2500" << endl;     //Showdown in the Wastes
            cout << u8" エアホース隊\t\t\t\t(2章-5節)\tN: HP  300(*5) | H/ExH: 1200(*5)" << endl;    //Showdown in the Wastes
            cout << u8" ヘビーマウントソルジャー\t\t(3章-4節)\tHP   4800" << endl;                   //Silent Edge
            cout << u8" 無式のシェルク\t\t\t\t(4章-3節)\tHP   3000" << endl;                         //Headquarters Under Siege
            cout << u8" 蒼きアスール\t\t\t\t(4章-4節)\tHP   4150 | 魔法耐性" << endl;       //Headquarters Under Siege
            cout << u8" ブラック・ウィドー\t\t\t(5章-4節)\tHP  10000" << endl;                       //Manor of Despair
            cout << u8" ブラック・ウィドーⅡ\t\t\t(6章-3節)\tHP   4500" << endl;                        //Deepground Strikes Back
            cout << u8" ＤＧＳ　トルーパー１ｓｔ\t\t(8章-1節-2話)\tHP   5400" << endl;               //Midgar Assault
            cout << u8" シュライク部隊\t\t\t\t(8章-1節-3話)\tHP    550(*6)" << endl;                  //Midgar Assault
            cout << u8" 朱のロッソ＆朱のロッソ　バースト\t(8章-2節-3話)\tHP  17000 | 魔法防御×2" << endl;      //Fight for the Central Complex
            cout << u8" ブラック・ウィドーＴＷ\t\t\t(9章-3節)\tHP   2000(*2)" << endl;                 //An Empire in Ruins
            cout << u8" 蒼きアスール　イークイップ２\t\t(9章-5節)\tHP   5000" << endl;                  //An Empire in Ruins
            cout << u8" 真・アスール\t\t\t\t(9章-5節)\tHP  13000" << endl;                           //An Empire in Ruins
            cout << u8" 漆黒の闇ネロ\t\t\t\t(10章-5節)\tHP  4000 + 2000*2 + 6500 | 魔法防御×2" << endl;  //Shinra's Dark Secret
            cout << u8" ドラゴンフライヤーＰＴ\t\t\t(11章-3節)\tHP  15000 | 魔法防御×2" << endl;                 //Beginnings
            cout << u8" ネロ・ラフレア\t\t\t\t(11章-4節)\tHP  12000" << endl;                         //Beginnings
            cout << u8" ネロ・ラフレアのバリア\t\t\t(11章-4節)\tHP   3500" << endl;                      //Beginnings
            cout << u8" ファランクス\t\t\t\t(11章-4節)\tHP   1000" << endl;                             //Beginnings
            cout << u8" ネロ・ウィングシューター\t\t(11章-4節)\tHP   3000 | 魔法防御×2" << endl;      //Beginnings
            cout << u8" 純白の帝王ヴァイス\t\t\t(11章-5節)\tHP  30000 | 魔法防御×4" << endl;      //Beginnings
            cout << u8" オメガの力宿りしヴァイス\t\t(11章-6節)\tHP  16000 | 魔法防御×4" << endl;      //Beginnings
            cout << u8" オメガコクーン\t\t\t\t(12章-2節)\tHP  15001 | 魔法防御×2" << endl;      //A Finale Chaotic
            cout << u8" オメガヴァイス\t\t\t\t(12章-2節)\tHP 240000" << endl;                         //A Finale Chaotic

            cout << u8"\n 魔法防御が 2 倍のターゲットは、サンダーのダメージ上限が 5000 になります。" << endl;
            cout << u8" 魔法防御が 4 倍のターゲットは、サンダーのダメージ上限が 2500 になります。" << endl;

            cout << "=====================================================================" << endl;
            break;
        default:
            language_error();
        }

        return 0;
    }


    //Function for calculating potential bullet damage before it is applied on bullet-specific hitbox.
    int preliminarydmg_bullets(int enemyDEF, int limiter)    //limiter arg is to check for (0.50 or 0.25 limiter)
    {
        baseSTRpower = (vincentSTR[vincentlevel] * physicalcoefficient.powerfloat) + powerconstant + 0.50;
        if (powerbooster == 0)
        {
            totalatkpower = (framepower + barrelpower);
        } //^No multiplication with (100 * 0.00999999977648) when a Power Booster is not present.
        else
        {
            totalatkpower = (framepower + barrelpower) * (powerboostervalues[powerbooster] + 100) * dividebyonehundred.zeropointzeroone;
        }

        preliminarydamage = (totalatkpower + 0.50) * (baseSTRpower + difficultymode_attackmodifier[difficultymode] + chaindamage + 100) * dividebyonehundred.zeropointzeroone;
        //When the bullet strikes, enemyDEF is checked
        baseDEFpower = (enemyDEF * defcoefficient.defensefloat) + defconstant.defensefloat + 0.50;
        preliminarydamage = (preliminarydamage + 0.50) * (100 - baseDEFpower) * dividebyonehundred.zeropointzeroone;
        if (limiter == 0)
        {
            preliminarydamage = preliminarydamage;
        }
        else if (limiter == 1)
        {
            preliminarydamage = (preliminarydamage * 0.50) + 0.50;
        }
        else
        {
            preliminarydamage = (preliminarydamage * 0.25) + 0.50;
        }

        switch (language)
        {
        case 0: //English
            cout << "Preliminary damage from one bullet: " << preliminarydamage << endl;

            if (preliminarydamage > 9999)
            {
                cout << "The preliminary single-bullet damage is capped at 9999." << endl;
                preliminarydamage = 9999;
            }
            break;
        case 1: //Japanese
            cout << u8"弾 1発あたりの理論上のダメージ： " << preliminarydamage << endl;

            if (preliminarydamage > 9999)
            {
                cout << u8"内部的に銃撃1回あたりのダメージは 9999 に制限されています。" << endl;
                preliminarydamage = 9999;
            }
            break;
        default:
            language_error();
        }

        if (bullet_type == 0) //Handgun
        {
            cout << stockphrases_bullets[language][0] << preliminarydamage << " | " << stockphrases_bullets[language][1] << preliminarydamage * 2 << " | " << stockphrases_bullets[language][2] << preliminarydamage * 3 << endl;
        }

        return 0;
    }


    //For bullet damage.
    int showhitboxdamage() {

        if (temporarystorage > 9999)
        {
            cout << " " << stockphrases_hitboxes[language][1] << temporarystorage << stockphrases_hitboxes[language][2] << endl;
            temporarystorage = 9999;
        }
        if (bullet_type == 0)
        {
            cout << "      " << stockphrases_bullets[language][0] << temporarystorage << " | " << stockphrases_bullets[language][1] << 2 * temporarystorage << " | " << stockphrases_bullets[language][2] << 3 * temporarystorage << endl;
        }
        else
        {
            cout << "      " << stockphrases_bullets[language][3] << temporarystorage << endl;
        }

        return 0;
    }


    //Function for selected boss enemy and data about the boss. bullet_type is 0, 1 or 2 (bullets hg/mg/r).
    int boss_selected()
    {

        if (selectedboss == 0) //Dragonfly, Ch1-3
        {

            damagelimiter = 0;
            //Info about Dragonfly, Ch1-3, Sea of Flames
            switch (language)
            {
            case 0: //English
                cout << "\n---DRAGONFLY--- [Ch1-3]" << endl;
                cout << " Normal Mode:       HP 1600" << endl;
                cout << " Hard/Ex Hard Mode: HP 2800" << endl;

                if (bossinfodump == 0)
                {
                    cout << "\n********************ADDITIONAL INFO********************" << endl;
                    cout << " Dragonfly has a DEF stat of 31." << endl;
                    cout << " Most enemies only have a DEF stat of 0." << endl;
                    cout << "*******************************************************" << endl;
                    bossinfodump = 1;
                }
                break;
            case 1: //Japanese
                cout << u8"\n ドラゴンフライヤー (1章-3節)" << endl;
                cout << u8" NORMAL：\tHP 1600" << endl;
                cout << u8" HARD/EX HARD：\tHP 2800" << endl;

                if (bossinfodump == 0)
                {
                    cout << u8"\n=======================追加情報=======================" << endl;
                    cout << u8" ドラゴンフライヤーのDEF値は 31 です。" << endl;
                    cout << u8" ほとんどの敵の防御値は 0 です。" << endl;
                    cout << u8"======================================================" << endl;
                    bossinfodump = 1;
                }
                break;
            default:
                language_error();
            }

            playerstatus();
            preliminarydmg_bullets(31, damagelimiter);

            cout << "----------------------------------------------------------" << endl;
            cout << stockphrases_hitboxes[language][0] << endl;

            switch (language)
            {
            case 0: //English
                cout << "\n (3.0) *Critical* Nose" << endl;    //60. Max damage: 9999.
                break;
            case 1: //Japanese
                cout << u8"\n (3.0) *クリティカル* 飛行機の鼻。" << endl;    //60. Max damage: 9999.
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 60 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();

            switch (language)
            {
            case 0: //English
                cout << "\n (2.5) Underside: Blue circles. Topside: Between rotor and tail." << endl;    //50. Max damage: 9999.
                break;
            case 1: //Japanese
                cout << u8"\n (2.5) 下：青い円。 上：ローターとテールの間。" << endl;    //50. Max damage: 9999.
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 50 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();

            switch (language)
            {
            case 0: //English
                cout << "\n (2.0) Topside: Tail and both wings" << endl;    //40. Max damage: 9999
                break;
            case 1: //Japanese
                cout << u8"\n (2.0) 上：しっぽと翼" << endl;    //40. Max damage: 9999
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 40 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();

            switch (language)
            {
            case 0: //English
                cout << "\n (1.0) Underside: Tail, both wings, area adjacent to blue circles" << endl;    //20. Max damage: 9999.
                break;
            case 1: //Japanese
                cout << u8"\n (1.0) 下側：尾、翼、青い円に隣接する領域" << endl;    //20. Max damage: 9999.
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 20 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();

            switch (language)
            {
            case 0: //English
                cout << "\n (0.7) Retractable legs" << endl;    //14. Max damage: 6999.
                break;
            case 1: //Japanese
                cout << u8"\n (0.7) 伸縮脚" << endl;    //14. Max damage: 6999.
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 14 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();

            switch (language)
            {
            case 0: //English
                cout << "\n (0.3) Topside: Rotor section and behind nose" << endl;    //6. Max damage: 2999.
                break;
            case 1: //Japanese
                cout << u8"\n (0.3) トップサイド：ローターセクションと機体先端後部" << endl;    //6. Max damage: 2999.
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 6 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();
        }


        else if (selectedboss == 1) //Dragonfly GL, Ch1-6
        {

            damagelimiter = 0;
            //Info about Dragonfly GL, Ch1-6, Sea of Flames.
            switch (language)
            {
            case 0: //English
                cout << "\n---DRAGONFLY GL--- [Ch1-6]" << endl;
                cout << " Normal Mode:       HP  7000" << endl;
                cout << " Hard/Ex Hard Mode: HP 12000" << endl;

                if (bossinfodump == 0)
                {
                    cout << "\n********************ADDITIONAL INFO********************" << endl;
                    cout << " Deplete 400 HP and the next hit will stagger the Dragonfly GL," << endl;
                    cout << " except when the boss is resistant to stagger for a few seconds." << endl;
                    cout << "*******************************************************" << endl;
                    bossinfodump = 1;
                }
                break;
            case 1: //Japanese
                cout << u8"\n ドラゴンフライヤーＧＬ (1章-6節)" << endl;
                cout << u8" NORMAL：\tHP  7000" << endl;
                cout << u8" HARD/EX HARD：\tHP 12000" << endl;

                if (bossinfodump == 0)
                {
                    cout << u8"\n=======================追加情報=======================" << endl;
                    cout << u8" ドラゴンフライヤーＧＬの上部の弱点を露呈するために、" << endl;
                    cout << u8" 一定量の HP を減らす必要があります。" << endl;
                    cout << u8"======================================================" << endl;
                    bossinfodump = 1;
                }
                break;
            default:
                language_error();
            }

            playerstatus();
            preliminarydmg_bullets(0, damagelimiter);

            cout << "----------------------------------------------------------" << endl;
            cout << stockphrases_hitboxes[language][0] << endl;

            switch (language)
            {
            case 0: //English
                cout << "\n (3.0) *Critical* Topside: Former rotor section" << endl;    //60. Max damage: 9999.
                break;
            case 1: //Japanese
                cout << u8"\n (3.0) *クリティカル* 上: ローターセクションのあった部分" << endl;    //60. Max damage: 9999.
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 60 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();

            switch (language)
            {
            case 0: //English
                cout << "\n (0.5) *Critical* Top of right leg (anterior) plus connected cylinder" << endl;    //10. Max damage: 4999.
                break;
            case 1: //Japanese
                cout << u8"\n (0.5) *クリティカル* 右脚の上部 (前) に接続された円柱" << endl;    //10. Max damage: 4999.
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 10 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();

            switch (language)
            {
            case 0: //English
                cout << "\n (0.5) Right retractable leg (anterior)" << endl;    //10. Max damage: 4999.
                break;
            case 1: //Japanese
                cout << u8"\n (0.5) 右伸縮脚 (前方)" << endl;    //10. Max damage: 4999.
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 10 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();

            switch (language)
            {
            case 0: //English
                cout << "\n (0.5) Cylinder connected to left leg (anterior)" << endl;    //10. Max damage: 4999.
                break;
            case 1: //Japanese
                cout << u8"\n (0.5) 左足に接続されたシリンダー (前方)" << endl;    //10. Max damage: 4999.
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 10 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();

            switch (language)
            {
            case 0: //English
                cout << "\n (0.1) Everything else" << endl;    //2. Max damage: 999.
                break;
            case 1: //Japanese
                cout << u8"\n (0.1) その他すべて" << endl;    //2. Max damage: 999.
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 2 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();
        }


        else if (selectedboss == 2) //Crimson Hound, Ch2-4
        {
            damagelimiter = 0;

            //Info about Crimson Hound, Ch2-4, Showdown in the Wastes
            switch (language)
            {
            case 0: //English
                cout << "\n---CRIMSON HOUND--- [Ch2-4]" << endl;
                cout << " Normal Mode:       HP 1500" << endl;
                cout << " Hard/Ex Hard Mode: HP 2500" << endl;

                if (bossinfodump == 0)
                {
                    cout << "\n********************ADDITIONAL INFO********************" << endl;
                    cout << " While the target is standing on a pillar their HP will" << endl;
                    cout << " not be depleted to below 1/10th of their max HP." << endl;
                    cout << " All hitboxes on the Crimson Hound have the same amount of defense." << endl;
                    cout << "*******************************************************" << endl;
                    bossinfodump = 1;
                }
                break;
            case 1: //Japanese
                cout << u8"\n クリムゾンハウンド (2章-4節)" << endl;
                cout << u8" NORMAL：\tHP 1500" << endl;
                cout << u8" HARD/EX HARD：\tHP 2500" << endl;

                if (bossinfodump == 0)
                {
                    cout << u8"\n=======================追加情報=======================" << endl;
                    cout << u8" 対象が柱に立っている間、" << endl;
                    cout << u8" HPが最大HPの1/10以下になりません。" << endl;
                    cout << u8" クリムゾンハウンドのすべての部位の防御力は同じです。" << endl;
                    cout << u8"======================================================" << endl;
                    bossinfodump = 1;
                }
                break;
            default:
                language_error();
            }
            playerstatus();
            preliminarydmg_bullets(0, damagelimiter);

            cout << "----------------------------------------------------------" << endl;
            cout << stockphrases_hitboxes[language][0] << endl;

            switch (language)
            {
            case 0: //English
                cout << "\n (1.0) All hitboxes" << endl;    //20. Max damage: 9999.
                break;
            case 1: //Japanese
                cout << u8"\n (1.0) すべての部位" << endl;    //20. Max damage: 9999.
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 20 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();

        }


        else if (selectedboss == 3) //Pegasus Riders, Ch2-5
        {
            damagelimiter = 0;

            //Info about Pegasus Riders, Ch2-5, Showdown in the Wastes
            switch (language)
            {
            case 0: //English
                cout << "\n---PEGASUS RIDERS--- [Ch2-5]" << endl;
                cout << " Normal Mode:       HP 300 each  (5* 300 = 1500)" << endl;
                cout << " Hard/Ex Hard Mode: HP 1200 each (5*1200 = 6000)" << endl;

                if (bossinfodump == 0)
                {
                    cout << "\n********************ADDITIONAL INFO********************" << endl;
                    cout << " Defeat all 5 targets to win the battle." << endl;
                    cout << " If you damage the last Pegasus Rider while they are charging at you" << endl;
                    cout << " there is a chance the battle will automatically end even if" << endl;
                    cout << " the target's HP was not depleted. The glitch can happen with" << endl;
                    cout << " all attack types." << endl;
                    cout << "*******************************************************" << endl;
                    bossinfodump = 1;
                }
                break;
            case 1: //Japanese
                cout << u8"\n エアホース隊 (2章-5節)" << endl;
                cout << u8" NORMAL：\tそれぞれ300HP。(5 * 300 = 1500)" << endl;
                cout << u8" HARD/EX HARD：\tそれぞれ1200HP。(5 * 1200 = 6000)" << endl;

                if (bossinfodump == 0)
                {
                    cout << u8"\n=======================追加情報=======================" << endl;
                    cout << u8" 5 つのターゲットをすべて倒すと勝利です。" << endl;
                    cout << u8" 突撃攻撃の最中に最後のエアホース隊にダメージを与えると、" << endl;
                    cout << u8" ターゲットの HP が尽きていなくても、自動的に戦闘が終了する場合があります。 " << endl;
                    cout << u8" この不具合は、すべての攻撃タイプで発生する可能性があります。" << endl;
                    cout << u8"======================================================" << endl;
                    bossinfodump = 1;
                }
                break;
            default:
                language_error();
            }

            playerstatus();
            preliminarydmg_bullets(0, damagelimiter);

            cout << "----------------------------------------------------------" << endl;
            cout << stockphrases_hitboxes[language][0] << endl;

            switch (language)
            {
            case 0: //English
                cout << "\n (4.0) *Critical* Vehicle nose & lower half of rider's platform" << endl;    //80. Max damage: 9999.
                break;
            case 1: //Japanese
                cout << u8"\n (4.0) *クリティカル* 車両の先端とライダーの乗車部の下半分" << endl;    //80. Max damage: 9999.
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 80 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();

            switch (language)
            {
            case 0: //English
                cout << "\n (1.0) Upper half of rider's platform" << endl;    //20. Max damage: 9999.
                break;
            case 1: //Japanese
                cout << u8"\n (1.0) ライダーの乗車部の上半分" << endl;    //20. Max damage: 9999.
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 20 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();

            switch (language)
            {
            case 0: //English
                cout << "\n (0.4) Rider + vehicle's backside fins" << endl;    //8. Max damage: 3999.
                break;
            case 1: //Japanese
                cout << u8"\n (0.4) ライダー + 車両後方の尾翼" << endl;    //8. Max damage: 3999.
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 8 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();

            switch (language)
            {
            case 0: //English
                cout << "\n (0.3) Frontal fins (attached to vehicle's nose)" << endl;    //6. Max damage: 2999.
                break;
            case 1: //Japanese
                cout << u8"\n (0.3) 前方翼 (車両の先端に付属)" << endl;    //6. Max damage: 2999.
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 6 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();
        }


        else if (selectedboss == 4) //Heavy Armored Soldier, Ch3-4
        {
            damagelimiter = 0;

            //Info about Heavy Armored Soldier, Ch3-4 (Silent Edge)
            switch (language)
            {
            case 0: //English
                cout << "\n---HEAVY ARMORED SOLDIER--- [Ch3-4]" << endl;
                cout << " HP 4800" << endl;

                if (bossinfodump == 0)
                {
                    cout << "\n********************ADDITIONAL INFO********************" << endl;
                    cout << " This boss has a STR stat of 50 but it only boosts the" << endl;
                    cout << " strength of their sword slashes. No other boss" << endl;
                    cout << " has a STR stat higher than 0." << endl;
                    cout << "*******************************************************" << endl;
                    bossinfodump = 1;
                }
                break;
            case 1: //Japanese
                cout << u8"\n ヘビーマウントソルジャー (3章-4節)" << endl;
                cout << u8" HP 4800" << endl;

                if (bossinfodump == 0)
                {
                    cout << u8"\n=======================追加情報=======================" << endl;
                    cout << u8" このボスの STR 値は 50 ですが、" << endl;
                    cout << u8" ソード スラッシュの強さだけが増加します。 " << endl;
                    cout << u8" STRが0以上のボスは他に存在しません。" << endl;
                    cout << u8"======================================================" << endl;
                    bossinfodump = 1;
                }
                break;
            default:
                language_error();
            }

            playerstatus();
            preliminarydmg_bullets(0, damagelimiter);

            cout << "----------------------------------------------------------" << endl;
            cout << stockphrases_hitboxes[language][0] << endl;

            switch (language)
            {
            case 0: //English
                cout << "\n (3.0) *Critical* Head" << endl;    //60. Max damage: 9999.
                break;
            case 1: //Japanese
                cout << u8"\n (3.0) *クリティカル* 頭部" << endl;    //60. Max damage: 9999.
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 60 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();

            switch (language)
            {
            case 0: //English
                cout << "\n (1.0) Body, bazooka tube & handguns" << endl;    //20. Max damage: 9999.
                break;
            case 1: //Japanese
                cout << u8"\n (1.0) 本体、バズーカの銃身、ハンドガン" << endl;    //20. Max damage: 9999.
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 20 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();

            switch (language)
            {
            case 0: //English
                cout << "\n (0.25) Shoulder pieces & sword" << endl;    //5. Max damage: 2499.
                break;
            case 1: //Japanese
                cout << u8"\n (0.25) ショルダー部分＆ソード" << endl;    //5. Max damage: 2499.
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 5 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();
        }


        else if (selectedboss == 5) //Shelke the Transparent, Ch4-3, Headquarters Under Siege
        {

            damagelimiter = 0;

            //Info about Shelke the Transparent, Ch4-3 (Headquarters Under Siege)
            switch (language)
            {
            case 0: //English
                cout << "\n---SHELKE THE TRANSPARENT--- [Ch4-3]" << endl;
                cout << " HP 3000" << endl;

                if (bossinfodump == 0)
                {
                    cout << "\n********************ADDITIONAL INFO********************" << endl;
                    cout << " Due to Shelke's low stature, automatic aim can strike critical hits" << endl;
                    cout << " if the barrel is in front of her head." << endl;
                    cout << "*******************************************************" << endl;
                    bossinfodump = 1;
                }
                break;
            case 1: //Japanese
                cout << u8"\n 無式のシェルク (4章-3節)" << endl;
                cout << u8" HP 3000" << endl;

                if (bossinfodump == 0)
                {
                    cout << u8"\n=======================追加情報=======================" << endl;
                    cout << u8" シェルクの身長が低いため、ヴィンセントの銃身が彼女の頭の前にある場合、" << endl;
                    cout << u8" 自動照準でもクリティカルヒットが発生する可能性があります。" << endl;
                    cout << u8"======================================================" << endl;
                    bossinfodump = 1;
                }
                break;
            default:
                language_error();
            }

            playerstatus();
            preliminarydmg_bullets(0, damagelimiter);

            cout << "----------------------------------------------------------" << endl;
            cout << stockphrases_hitboxes[language][0] << endl;

            switch (language)
            {
            case 0: //English
                cout << "\n (2.0) *Critical* Head" << endl;    //40. Max damage: 9999.
                break;
            case 1: //Japanese
                cout << u8"\n (2.0) *クリティカル* 頭部" << endl;    //40. Max damage: 9999.
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 40 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();

            switch (language)
            {
            case 0: //English
                cout << "\n (1.0) Remainder of body" << endl;    //20. Max damage: 9999.
                break;
            case 1: //Japanese
                cout << u8"\n (1.0) 体の他の部分。" << endl;    //20. Max damage: 9999.
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 20 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();
        }


        else if (selectedboss == 6) //Azul the Cerulean      (Ch4-4)   HP 4150
        {

            damagelimiter = 1;

            //Info about Azul the Cerulean, Ch4-4 (Headquarters Under Siege)
            switch (language)
            {
            case 0: //English
                cout << "\n---AZUL THE CERULEAN--- [Ch4-4]" << endl;
                cout << " HP 4150" << endl;

                if (bossinfodump == 0)
                {
                    cout << "\n********************ADDITIONAL INFO********************" << endl;
                    cout << " If a drum can is exploded in Azul's vicinity it will cause" << endl;
                    cout << " random damage in the range of 695 - 710." << endl;
                    cout << " Six explosions are always enough to take down Azul." << endl;

                    cout << " \n Even before the hitbox-specific defense values are" << endl;
                    cout << " considered, damage from bullets is halved on Azul." << endl;
                    cout << "*******************************************************" << endl;
                    bossinfodump = 1;
                }
                break;
            case 1: //Japanese
                cout << u8"\n 蒼きアスール (4章-4節)" << endl;
                cout << u8" HP 4150" << endl;

                if (bossinfodump == 0)
                {
                    cout << u8"\n=======================追加情報=======================" << endl;
                    cout << u8" アスールの近くで樽を爆発させると、" << endl;
                    cout << u8" 695 ～ 710 の範囲のランダムなダメージが与えられます。" << endl;
                    cout << u8" 乱数を考慮したうえで、爆発を6回ヒットさせれば確実に倒すことができます。" << endl;
                    cout << u8" アスールに与える銃撃ダメージは、通常の 1/2 になります。" << endl;
                    cout << u8"======================================================" << endl;
                    bossinfodump = 1;
                }
                break;
            default:
                language_error();
            }

            playerstatus();
            preliminarydmg_bullets(0, damagelimiter);

            cout << "----------------------------------------------------------" << endl;
            cout << stockphrases_hitboxes[language][0] << endl;

            switch (language)
            {
            case 0: //English
                cout << "\n (0.4) *Critical* Head" << endl;    //8. Max damage: 3999.
                break;
            case 1: //Japanese
                cout << u8"\n (0.4) *クリティカル* 頭部" << endl;    //8. Max damage: 3999.
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 8 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();

            switch (language)
            {
            case 0: //English
                cout << "\n (0.2) Torso & legs" << endl;    //4. Max damage: 1999.
                break;
            case 1: //Japanese
                cout << u8"\n (0.2) 胴体と脚" << endl;    //4. Max damage: 1999.
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 4 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();

            switch (language)
            {
            case 0: //English
                cout << "\n (0.1) Arms & feet" << endl;    //2. Max damage: 999.
                break;
            case 1: //Japanese
                cout << u8"\n (0.1) 両腕と足" << endl;    //2. Max damage: 999.
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 2 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();
        }


        else if (selectedboss == 7) //Black Widow            (Ch5-4)   HP 10000
        {

            damagelimiter = 0;

            //Info about Black Widow, Ch5-4 (Manor of Despair)
            switch (language)
            {
            case 0: //English
                cout << "\n---BLACK WIDOW--- [Ch5-4]" << endl;
                cout << " HP 10000" << endl;

                if (bossinfodump == 0)
                {
                    cout << "\n********************ADDITIONAL INFO********************" << endl;
                    cout << " The enemy's weak spot can be exposed by making them drop from the ceiling." << endl;
                    cout << " While the Black Widow's belly is exposed, Fire- and Thunder magic will not work." << endl;
                    cout << "*******************************************************" << endl;
                    bossinfodump = 1;
                }
                break;
            case 1: //Japanese
                cout << u8"\n ブラック・ウィドー (5章-4節)" << endl;
                cout << u8" HP 10000" << endl;

                if (bossinfodump == 0)
                {
                    cout << u8"\n=======================追加情報=======================" << endl;
                    cout << u8" 敵を天井から落とすと腹部の弱点が露出します。" << endl;
                    cout << u8" ブラック・ウィドーの腹部が露出している間、ファイアとサンダーに耐性が発生します。" << endl;
                    cout << u8"======================================================" << endl;
                    bossinfodump = 1;
                }
                break;
            default:
                language_error();
            }

            playerstatus();
            preliminarydmg_bullets(0, damagelimiter);

            cout << "----------------------------------------------------------" << endl;
            cout << stockphrases_hitboxes[language][0] << endl;

            switch (language)
            {
            case 0: //English
                cout << "\n (4.0) *Critical* Underside/belly" << endl;    //80. Max damage: 9999.
                break;
            case 1: //Japanese
                cout << u8"\n (4.0) *クリティカル* 下側/腹部" << endl;    //80. Max damage: 9999.
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 80 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();

            switch (language)
            {
            case 0: //English
                cout << "\n (0.3) Remainder of body" << endl;    //6. Max damage: 2999.
                break;
            case 1: //Japanese
                cout << u8"\n (0.3) 体の他の部分。" << endl;    //6. Max damage: 2999.
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 6 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();

        }


        else if (selectedboss == 8) //Black Widow II         (Ch6-3)   HP 4500
        {

            damagelimiter = 0;

            //Info about Black Widow II, Ch6-3 (Deepground Strikes Back)
            switch (language)
            {
            case 0: //English
                cout << "\n---BLACK WIDOW II--- [Ch6-3]" << endl;
                cout << " HP 4500" << endl;

                if (bossinfodump == 0)
                {
                    cout << "\n********************ADDITIONAL INFO********************" << endl;
                    cout << " Same as Black Widow in Shinra Manor (Ch5-4) except now the HP is lower." << endl;
                    cout << "*******************************************************" << endl;
                    bossinfodump = 1;
                }
                break;
            case 1: //Japanese
                cout << u8"\n ブラック・ウィドーⅡ (6章-3節)" << endl;
                cout << u8" HP 4500" << endl;

                if (bossinfodump == 0)
                {
                    cout << u8"\n=======================追加情報=======================" << endl;
                    cout << u8" HPが低いことを除いて、" << endl;
                    cout << u8" 神羅屋敷のブラック・ウィドウ(5章-4節)と同じステータスを持ちます。" << endl;
                    cout << u8"======================================================" << endl;
                    bossinfodump = 1;
                }
                break;
            default:
                language_error();
            }

            playerstatus();
            preliminarydmg_bullets(0, damagelimiter);

            cout << "----------------------------------------------------------" << endl;
            cout << stockphrases_hitboxes[language][0] << endl;

            switch (language)
            {
            case 0: //English
                cout << "\n (4.0) *Critical* Underside/belly" << endl;    //80. Max damage: 9999.
                break;
            case 1: //Japanese
                cout << u8"\n (4.0) *クリティカル* 下側/腹部" << endl;    //80. Max damage: 9999.
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 80 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();

            switch (language)
            {
            case 0: //English
                cout << "\n (0.3) Remainder of body" << endl;    //6. Max damage: 2999.
                break;
            case 1: //Japanese
                cout << u8"\n (0.3) 体の他の部分。" << endl;    //6. Max damage: 2999.
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 6 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();
        }


        else if (selectedboss == 9) //Deepground Commander   (Ch8-1-2) HP 5400
        {

            damagelimiter = 0;

            //Info about Deepground Commander, Ch8-1-2 (Midgar Assault)
            switch (language)
            {
            case 0: //English
                cout << "\n---DEEPGROUND COMMANDER--- [Ch8-1-2]" << endl;
                cout << " HP 5400" << endl;

                if (bossinfodump == 0)
                {
                    cout << "\n********************ADDITIONAL INFO********************" << endl;
                    cout << " The target is initially invulnerable. This status drops" << endl;
                    cout << " soon as they've reached the position to start firing." << endl;
                    cout << " The DG Commander in the tunnel that follows have the same" << endl;
                    cout << " hitbox defenses but their HP is only 380." << endl;
                    cout << "*******************************************************" << endl;
                    bossinfodump = 1;
                }
                break;
            case 1: //Japanese
                cout << u8"\n ＤＧＳ　トルーパー１ｓｔ (8章-1節-2話)" << endl;
                cout << u8" HP 5400" << endl;

                if (bossinfodump == 0)
                {
                    cout << u8"\n=======================追加情報=======================" << endl;
                    cout << u8" ターゲットは戦闘開始直後、無敵状態です。" << endl;
                    cout << u8" 攻撃開始位置に到達すると、ダメージが与えられるようになります。" << endl;
                    cout << u8" 次のトンネルにいる隊長も同じ部位と防御力を持ちますが、" << endl;
                    cout << u8" HP は 380 しかありません。" << endl;
                    cout << u8"======================================================" << endl;
                    bossinfodump = 1;
                }
                break;
            default:
                language_error();
            }

            playerstatus();
            preliminarydmg_bullets(0, damagelimiter);

            cout << "----------------------------------------------------------" << endl;
            cout << stockphrases_hitboxes[language][0] << endl;

            switch (language)
            {
            case 0: //English
                cout << "\n (3.0) *Critical* Head" << endl;    //60. Max damage: 9999.
                break;
            case 1: //Japanese
                cout << u8"\n (3.0) *クリティカル* 頭部" << endl;    //60. Max damage: 9999.
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 60 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();

            switch (language)
            {
            case 0: //English
                cout << "\n (1.0) Remainder of body" << endl;    //20. Max damage: 9999.
                break;
            case 1: //Japanese
                cout << u8"\n (1.0) 体の他の部分。" << endl;    //20. Max damage: 9999.
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 20 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();
        }


        else if (selectedboss == 10) //Shrikes       (Ch8-1-3) 6*550 = HP 3300
        {

            damagelimiter = 0;

            //Info about Shrikes, Ch8-1-3, Midgar Assault
            switch (language)
            {
            case 0: //English
                cout << "\n---SHRIKES--- [Ch8-1-3]" << endl;
                cout << " HP 550 each  (6*550 = 3300)" << endl;

                if (bossinfodump == 0)
                {
                    cout << "\n********************ADDITIONAL INFO********************" << endl;
                    cout << " Defeat all 6 targets to win the battle." << endl;
                    cout << " A common glitch with the Shrikes is that when you deal" << endl;
                    cout << " a killing shot, even when using just one bullet, is" << endl;
                    cout << " that the game always displays damage digits equivalent to" << endl;
                    cout << " striking the main body (1.0 damage modifier), regardless" << endl;
                    cout << " of where the shot landed. The true damage is still dealt." << endl;

                    cout << "\n The jetpack engine and its horizontal wings are the target's" << endl;
                    cout << " weakest part but they do not count as a critical hit." << endl;
                    cout << "*******************************************************" << endl;
                    bossinfodump = 1;
                }
                break;
            case 1: //Japanese
                cout << u8"\n シュライク部隊 (8章-1節-3話)" << endl;
                cout << u8" それぞれ550HP。(6 * 550 = 3300)" << endl;

                if (bossinfodump == 0)
                {
                    cout << u8"\n=======================追加情報=======================" << endl;
                    cout << u8" 6 つのターゲットをすべて倒すと勝利です。" << endl;
                    cout << u8" ジェットパック エンジンと水平翼が弱点ですが、" << endl;
                    cout << u8" クリティカル ヒットにはなりません。" << endl;
                    cout << u8"======================================================" << endl;
                    bossinfodump = 1;
                }
                break;
            default:
                language_error();
            }

            playerstatus();
            preliminarydmg_bullets(0, damagelimiter);

            cout << "----------------------------------------------------------" << endl;
            cout << stockphrases_hitboxes[language][0] << endl;

            switch (language)
            {
            case 0: //English
                cout << "\n (4.0) Jetpack engine & wings" << endl;    //80. Max damage: 9999.
                break;
            case 1: //Japanese
                cout << u8"\n (4.0) ジェットパック エンジンと翼" << endl;    //80. Max damage: 9999.
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 80 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();

            switch (language)
            {
            case 0: //English
                cout << "\n (3.0) *Critical* Head" << endl;    //60. Max damage: 9999.
                break;
            case 1: //Japanese
                cout << u8"\n (3.0) *クリティカル* 頭部" << endl;    //60. Max damage: 9999.
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 60 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();

            switch (language)
            {
            case 0: //English
                cout << "\n (2.0) Vertical fins of jetpack" << endl;    //40. Max damage: 9999.
                break;
            case 1: //Japanese
                cout << u8"\n (2.0) ジェットパックの垂直尾翼" << endl;    //40. Max damage: 9999.
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 40 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();

            switch (language)
            {
            case 0: //English
                cout << "\n (1.0) Remainder of body" << endl;    //20. Max damage: 9999.
                break;
            case 1: //Japanese
                cout << u8"\n (1.0) 体の他の部分。" << endl;    //20. Max damage: 9999.
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 20 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();
        }


        else if (selectedboss == 11) //Rosso the Crimson, Ch8-2-3, Fight for the Central Complex
        {

            damagelimiter = 0;

            //Info about Rosso the Crimson, Ch8-2-3 (Fight for the Central Complex)
            switch (language)
            {
            case 0: //English
                cout << "\n---ROSSO THE CRIMSON & BLOODBURST ROSSO--- [Ch8-2-3]" << endl;
                cout << " HP 17000" << endl;

                if (bossinfodump == 0)
                {
                    cout << "\n********************ADDITIONAL INFO********************" << endl;
                    cout << " Enters second battle phase when 4080 or more HP has been depleted." << endl;
                    cout << " The second- and first form use the same HP bar." << endl;
                    cout << " Automatic aim focuses on region with damage modifier of 1.0" << endl;
                    cout << "*******************************************************" << endl;
                    bossinfodump = 1;
                }
                break;
            case 1: //Japanese
                cout << u8"\n 朱のロッソ＆朱のロッソ　バースト (8章-2節-3話)" << endl;
                cout << u8" HP 17000" << endl;

                if (bossinfodump == 0)
                {
                    cout << u8"\n=======================追加情報=======================" << endl;
                    cout << u8" HPが12920以下になると朱のロッソ バーストに変化します。" << endl;
                    cout << u8" その際HPは回復せず、朱のロッソ時に与えたダメージを引き継ぎます。" << endl;
                    cout << u8"======================================================" << endl;
                    bossinfodump = 1;
                }
                break;
            default:
                language_error();
            }

            playerstatus();
            preliminarydmg_bullets(0, damagelimiter);

            cout << "----------------------------------------------------------" << endl;
            cout << stockphrases_hitboxes[language][0] << endl;

            switch (language)
            {
            case 0: //English
                cout << "\n (2.5) *Critical* Head" << endl;    //50. Max damage: 9999.
                break;
            case 1: //Japanese
                cout << u8"\n (2.5) *クリティカル* 頭部" << endl;    //50. Max damage: 9999.
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 50 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();

            switch (language)
            {
            case 0: //English
                cout << "\n (1.0) Torso" << endl;    //20. Max damage: 9999.
                break;
            case 1: //Japanese
                cout << u8"\n (1.0) 胴体" << endl;    //20. Max damage: 9999.
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 20 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();

            switch (language)
            {
            case 0: //English
                cout << "\n (0.8) Thighs" << endl;    //16. Max damage: 7999.
                break;
            case 1: //Japanese
                cout << u8"\n (0.8) 太もも" << endl;    //16. Max damage: 7999.
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 16 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();

            switch (language)
            {
            case 0: //English
                cout << "\n (0.7) Arms & lower legs" << endl;    //14. Max damage: 6999.
                break;
            case 1: //Japanese
                cout << u8"\n (0.7) 腕と下脚" << endl;    //14. Max damage: 6999.
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 14 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();

            switch (language)
            {
            case 0: //English
                cout << "\n (0.6) Feet" << endl;    //12. Max damage: 5999.
                break;
            case 1: //Japanese
                cout << u8"\n (0.6) 足" << endl;    //12. Max damage: 5999.
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 12 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();
        }


        else if (selectedboss == 12) //Black Widow TW         (Ch9-3)   2*2000 = HP 4000
        {

            damagelimiter = 0;

            //Info about Black Widow TW, Ch9-3 (An Empire in Ruins)
            switch (language)
            {
            case 0: //English
                cout << "\n---BLACK WIDOW TW--- [Ch9-3]" << endl;
                cout << " HP 2000(*2)" << endl;

                if (bossinfodump == 0)
                {
                    cout << "\n********************ADDITIONAL INFO********************" << endl;
                    cout << " Same as the previous Black Widows but this time there are two of them." << endl;
                    cout << " One on the ground and one on the ceiling. The Black Widow on the ceiling" << endl;
                    cout << " can be knocked down to reveal its weak spot. Each BW has 2000 HP." << endl;
                    cout << "*******************************************************" << endl;
                    bossinfodump = 1;
                }
                break;
            case 1: //Japanese
                cout << u8"\n ブラック・ウィドーＴＷ (9章-3節)" << endl;
                cout << u8" HP 2000(*2)" << endl;

                if (bossinfodump == 0)
                {
                    cout << u8"\n=======================追加情報=======================" << endl;
                    cout << u8" 前回のブラック・ウィドウと同じ性能ですが、今回は2体出現します。地面に1体、天井に1体。" << endl;
                    cout << u8" 天井のブラック・ウィドーはノックダウンすることで腹部の弱点が露出します。" << endl;
                    cout << u8" 各ブラック・ウィドーの HP は 2000 です。" << endl;
                    cout << u8"======================================================" << endl;
                    bossinfodump = 1;
                }
                break;
            default:
                language_error();
            }

            playerstatus();
            preliminarydmg_bullets(0, damagelimiter);

            cout << "----------------------------------------------------------" << endl;
            cout << stockphrases_hitboxes[language][0] << endl;

            switch (language)
            {
            case 0: //English
                cout << "\n (4.0) *Critical* Underside/belly" << endl;    //80. Max damage: 9999.
                break;
            case 1: //Japanese
                cout << u8"\n (4.0) *クリティカル* 下側/腹部" << endl;    //80. Max damage: 9999.
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 80 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();

            switch (language)
            {
            case 0: //English
                cout << "\n (0.3) Remainder of body" << endl;    //6. Max damage: 2999.
                break;
            case 1: //Japanese
                cout << u8"\n (0.3) 体の他の部分。" << endl;    //6. Max damage: 2999.
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 6 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();
        }


        else if (selectedboss == 13) //Neo Azul           (Ch9-5)   HP 5000
        {

            damagelimiter = 1;

            //Info about Neo Azul, Ch9-5 (An Empire in Ruins)
            switch (language)
            {
            case 0: //English
                cout << "\n---NEO AZUL--- [Ch9-5]" << endl;
                cout << " HP 5000" << endl;

                if (bossinfodump == 0)
                {
                    cout << "Bullet damage on Azul is halved here, just like in Ch4." << endl;
                    bossinfodump = 1;
                }
                break;
            case 1: //Japanese
                cout << u8"\n 蒼きアスール　イークイップ２ (9章-5節)" << endl;
                cout << u8" HP 5000" << endl;

                if (bossinfodump == 0)
                {
                    cout << u8"\n=======================追加情報=======================" << endl;
                    cout << u8" 4章と同様に、アスールに与える銃撃ダメージは通常の 1/2 になります。" << endl;
                    cout << u8"======================================================" << endl;
                    bossinfodump = 1;
                }
                break;
            default:
                language_error();
            }

            playerstatus();
            preliminarydmg_bullets(0, damagelimiter);

            cout << "----------------------------------------------------------" << endl;
            cout << stockphrases_hitboxes[language][0] << endl;

            switch (language)
            {
            case 0: //English
                cout << "\n (1.5) *Critical* Head" << endl;    //30. Max damage: 9999.
                break;
            case 1: //Japanese
                cout << u8"\n (1.5) *クリティカル* 頭部" << endl;    //30. Max damage: 9999.
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 30 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();

            switch (language)
            {
            case 0: //English
                cout << "\n (1.0) Everything except for head & upper torso" << endl;    //20. Max damage: 9999.
                break;
            case 1: //Japanese
                cout << u8"\n (1.0) 頭と上半身を除くすべて" << endl;    //20. Max damage: 9999.
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 20 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();

            switch (language)
            {
            case 0: //English
                cout << "\n (0.8) Upper torso" << endl;    //16. Max damage: 7999.
                break;
            case 1: //Japanese
                cout << u8"\n (0.8) 上半身" << endl;    //16. Max damage: 7999.
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 16 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();
        }


        else if (selectedboss == 14) //Arch Azul              (Ch9-5)   HP 13000
        {

            //Info about Arch Azul, Ch9-5 (An Empire in Ruins)
            switch (language)
            {
            case 0: //English
                cout << "\n---ARCH AZUL--- [Ch9-5]" << endl;
                cout << " HP 13000" << endl;

                cout << "\n Bullets will always cause 0 damage on Arch Azul." << endl;
                cout << " Melee and/or magic must be used to defeat this target." << endl;
                cout << "\n The game does check specific hitbox-defense modifiers when a bullet lands," << endl;
                cout << " but the potential damage has already been multiplied with 0 by that point." << endl;
                break;
            case 1: //Japanese
                cout << u8"\n 真・アスール (9章-5節)" << endl;
                cout << u8" HP 13000" << endl;

                cout << u8"\n 真・アスールには、銃撃でダメージを与えることができません。" << endl;
                cout << u8" 近接攻撃または魔法を使用する必要があります。" << endl;
                break;
            default:
                language_error();
            }
        }


        else if (selectedboss == 15) //Nero the Sable         (Ch10-5)  HP 14500 (effectively)
        {

            damagelimiter = 0;

            //Info about Nero the Sable, Ch10-5 (Shinra's Dark Secret)
            switch (language)
            {
            case 0: //English
                cout << "\n---NERO THE SABLE--- [Ch10-5]" << endl;
                cout << " Phase 1: HP  4000" << endl;
                cout << " Phase 2: HP  2000(*2)" << endl;
                cout << " Phase 3: HP  6500" << endl;
                cout << " Total  : HP 14500" << endl;

                if (bossinfodump == 0)
                {
                    cout << "\n********************ADDITIONAL INFO********************" << endl;
                    cout << " In the battle's 2nd phase, when there are three Neros about," << endl;
                    cout << " only two need to have their HP depleted. Each one has 2000 HP." << endl;
                    cout << " The remaining Nero will have their HP reset to 6500 for" << endl;
                    cout << " the 3rd phase. Every iteration of Nero share the same" << endl;
                    cout << " damage modifiers for bullet damage on specific hitboxes." << endl;
                    cout << "*******************************************************" << endl;
                    bossinfodump = 1;
                }
                break;
            case 1: //Japanese
                cout << u8"\n 漆黒の闇ネロ (10章-5節)" << endl;
                cout << u8" フェーズ 1：\tHP  4000" << endl;
                cout << u8" フェーズ 2：\tHP  2000 (*2)" << endl;
                cout << u8" フェーズ 3：\tHP  6500" << endl;
                cout << u8" 合計：\t\tHP 14500" << endl;

                if (bossinfodump == 0)
                {
                    cout << u8"\n=======================追加情報=======================" << endl;
                    cout << u8" フェーズ2でネロが分身した場合、分身2体のHPを削る必要があります。" << endl;
                    cout << u8" 分身のHPは1体あたり 2000 です。" << endl;
                    cout << u8" 残りの1体は、フェーズ3に移行した時点でHPが 6500 にリセットされます。" << endl;
                    cout << u8" 特定の部位に対する銃撃ダメージの修正値は、" << endl;
                    cout << u8" どのネロに対しても共通の値が使用されています。" << endl;
                    cout << u8"======================================================" << endl;
                    bossinfodump = 1;
                }
                break;
            default:
                language_error();
            }

            playerstatus();
            preliminarydmg_bullets(0, damagelimiter);

            cout << "----------------------------------------------------------" << endl;
            cout << stockphrases_hitboxes[language][0] << endl;

            switch (language)
            {
            case 0: //English
                cout << "\n (3.0) *Critical* Head" << endl;    //60. Max damage: 9999.
                break;
            case 1: //Japanese
                cout << u8"\n (3.0) *クリティカル* 頭部" << endl;    //60. Max damage: 9999.
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 60 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();

            switch (language)
            {
            case 0: //English
                cout << "\n (1.0) Remainder of body" << endl;    //20. Max damage: 9999.
                break;
            case 1: //Japanese
                cout << u8"\n (1.0) 体の他の部分。" << endl;    //20. Max damage: 9999.
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 20 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();
        }


        else if (selectedboss == 16) //Dragonfly PT           (Ch11-3)  HP 15000
        {

            damagelimiter = 1;

            //Info about Dragonfly PT, Ch11-3 (Beginnings)
            switch (language)
            {
            case 0: //English
                cout << "\n---DRAGONFLY PT--- [Ch11-3]" << endl;
                cout << " HP 15000" << endl;

                if (bossinfodump == 0)
                {
                    cout << "\n********************ADDITIONAL INFO********************" << endl;
                    cout << " Dragonfly PT has a DEF stat of 31, same as the Dragonfly in Ch1-3." << endl;
                    cout << " This boss however also halves the damage from all damage types even" << endl;
                    cout << " before the hitbox-specific defense values have been taken into account." << endl;
                    cout << "*******************************************************" << endl;
                    bossinfodump = 1;
                }
                break;
            case 1: //Japanese
                cout << u8"\n ドラゴンフライヤーＰＴ (11章-3節)" << endl;
                cout << u8" HP 15000" << endl;

                if (bossinfodump == 0)
                {
                    cout << u8"\n=======================追加情報=======================" << endl;
                    cout << u8" ドラゴンフライヤーＰＴの DEF 値は 31 で、" << endl;
                    cout << u8" 1章-3節のドラゴンフライヤーと同じ数値です。" << endl;
                    cout << u8" ただしこのボスは、" << endl;
                    cout << u8" 部位ごとの防御力計算よりも先にすべての種類の銃撃ダメージを 1/2 とします。" << endl;
                    cout << u8"======================================================" << endl;
                    bossinfodump = 1;
                }
                break;
            default:
                language_error();
            }
            playerstatus();
            preliminarydmg_bullets(31, damagelimiter);    //Damage of all types is halved via the 0.50 limiter


            cout << "----------------------------------------------------------" << endl;
            cout << stockphrases_hitboxes[language][0] << endl;

            switch (language)
            {
            case 0: //English
                cout << "\n (3.0) *Critical* Nose" << endl;    //60. Max damage: 9999.
                break;
            case 1: //Japanese
                cout << u8"\n (3.0) *クリティカル* 飛行機の鼻。" << endl;    //60. Max damage: 9999.
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 60 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();

            switch (language)
            {
            case 0: //English
                cout << "\n (2.5) Underside: Blue circles. Topside: Rotor and inner tail." << endl;    //50. Max damage: 9999.
                break;
            case 1: //Japanese
                cout << u8"\n (2.5) 下: 青い円。 上：ローターとインナーテール。" << endl;    //50. Max damage: 9999.
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 50 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();

            switch (language)
            {
            case 0: //English
                cout << "\n (2.0) Topside: Tail and both wings" << endl;    //40. Max damage: 9999.
                break;
            case 1: //Japanese
                cout << u8"\n (2.0) 上：しっぽと翼" << endl;    //40. Max damage: 9999.
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 40 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();

            switch (language)
            {
            case 0: //English
                cout << "\n (1.0) Underside: Tail, both wings, area adjacent to blue circles" << endl;    //20. Max damage: 9999.
                break;
            case 1: //Japanese
                cout << u8"\n (1.0) 下側：尾、翼、青い円に隣接する領域" << endl;    //20. Max damage: 9999.
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 20 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();

            switch (language)
            {
            case 0: //English
                cout << "\n (0.7) Retractable legs" << endl;    //14. Max damage: 6999.
                break;
            case 1: //Japanese
                cout << u8"\n (0.7) 伸縮脚" << endl;    //14. Max damage: 6999.
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 14 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();

            switch (language)
            {
            case 0: //English
                cout << "\n (0.3) Topside: Near rotor section and behind nose" << endl;    //6. Max damage: 2999.
                break;
            case 1: //Japanese
                cout << u8"\n (0.3) 上: ローター セクションの近くと機体先端後部" << endl;    //6. Max damage: 2999.
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 6 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();
        }


        else if (selectedboss == 17) //Arachnero              (Ch11-4)  HP 12000
        {

            damagelimiter = 0;

            //Info about Arachnero, Ch11-4 (Beginnings)
            switch (language)
            {
            case 0: //English
                cout << "\n---ARACHNERO--- [Ch11-4]" << endl;
                cout << " Arachnero:    HP 12000" << endl;
                cout << " Shield:       HP  3500" << endl;
                cout << " Phalanxphere: HP  1000" << endl;

                if (bossinfodump == 0)
                {
                    cout << "\n********************ADDITIONAL INFO********************" << endl;
                    cout << " Battle is won either by depleting 12000 HP from the main body" << endl;
                    cout << " or by destroying three of the legs. Each leg is worth 4000 HP." << endl;
                    cout << " \n The floating crystals rods that shoot lasers are officially" << endl;
                    cout << " known as 'Phalanxpheres'. These rods can only be damaged while" << endl;
                    cout << " it is charging for- or firing its laser." << endl;
                    cout << "*******************************************************" << endl;
                    bossinfodump = 1;
                }
                break;
            case 1: //Japanese
                cout << u8"\n ネロ・ラフレア (11章-4節)" << endl;
                cout << u8" ネロ・ラフレア：\tHP 12000" << endl;
                cout << u8" バリア：\t\tHP  3500" << endl;
                cout << u8" ファランクス：\t\tHP  1000" << endl;

                if (bossinfodump == 0)
                {
                    cout << u8"\n=======================追加情報=======================" << endl;
                    cout << u8" 本体のHPを12000減らすか、脚を3本破壊することで勝利です。" << endl;
                    cout << u8" 脚1本につきネロ・ラフレアの HP 4000 にあたります。" << endl;
                    cout << u8" \n レーザーを発射する浮遊クリスタルロッドは、" << endl;
                    cout << u8" 正式には「ファランクス」として知られています。 " << endl;
                    cout << u8" これらのロッドには、レーザーの充電中または発射中にのみダメージを与えられます。" << endl;
                    cout << u8"======================================================" << endl;
                    bossinfodump = 1;
                }
                break;
            default:
                language_error();
            }

            playerstatus();
            preliminarydmg_bullets(0, damagelimiter);

            //Arachnero
            cout << "----------------------------------------------------------" << endl;
            cout << stockphrases_hitboxes[language][0] << endl;

            switch (language)
            {
            case 0: //English
                cout << "\n (3.0) *Critical* Nero's head" << endl;    //60. Max damage: 9999.
                break;
            case 1: //Japanese
                cout << u8"\n (3.0) *クリティカル* ネロの頭" << endl;    //60. Max damage: 9999.
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 60 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();

            switch (language)
            {
            case 0: //English
                cout << "\n (1.0) *Critical* Arachnid legs" << endl;    //20. Max damage: 9999.
                break;
            case 1: //Japanese
                cout << u8"\n (1.0) *クリティカル* クモの脚" << endl;    //20. Max damage: 9999.
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 20 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();

            switch (language)
            {
            case 0: //English
                cout << "\n (1.0) Arachnid abdomen- & face, Nero's torso & new lower half" << endl;    //20. Max damage: 9999.
                break;
            case 1: //Japanese
                cout << u8"\n (1.0) クモの腹部と顔、ネロの胴体と新しい下半身" << endl;    //20. Max damage: 9999.
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 20 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();

            switch (language)
            {
            case 0: //English
                cout << "\n (0.8) Spine connecting human Nero & arachnid body" << endl;    //16. Max damage: 7999.
                break;
            case 1: //Japanese
                cout << u8"\n (0.8) ネロとクモの体をつなぐ背骨" << endl;    //16. Max damage: 7999.
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 16 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();

            switch (language)
            {
            case 0: //English
                cout << "\n (0.7) Nero's human arms" << endl;    //14. Max damage: 6999.
                break;
            case 1: //Japanese
                cout << u8"\n (0.7) ネロの腕" << endl;    //14. Max damage: 6999.
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 14 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();

            switch (language)
            {
            case 0: //English
                cout << "\n (0.2) Nero's wings" << endl;    //4. Max damage: 1999.
                break;
            case 1: //Japanese
                cout << u8"\n (0.2) ネロの翼" << endl;    //4. Max damage: 1999.
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 4 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();

            //Shield & Phalanxphere
            switch (language)
            {
            case 0: //English
                cout << "\n (1.0) Shield & Phalanxphere defense" << endl;    //20. Max damage: 9999.
                break;
            case 1: //Japanese
                cout << u8"\n (1.0) バリア&ファランクス。" << endl;    //20. Max damage: 9999.
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 20 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();
        }


        else if (selectedboss == 18) //Gorgonero              (Ch11-4)  HP 3000
        {

            damagelimiter = 0;

            //Info about Gorgonero, Ch11-4 (Beginnings)
            switch (language)
            {
            case 0: //English
                cout << "\n---GORGONERO--- [Ch11-4]" << endl;
                cout << " HP 3000" << endl;

                if (bossinfodump == 0)
                {
                    cout << "\n********************ADDITIONAL INFO********************" << endl;
                    cout << "\n Same hitbox defense as Nero the Sable (Ch10-5)." << endl;
                    cout << "*******************************************************" << endl;
                    bossinfodump = 1;
                }
                break;
            case 1: //Japanese
                cout << u8"\n ネロ・ウィングシューター (11章-4節)" << endl;
                cout << u8" HP 3000" << endl;

                if (bossinfodump == 0)
                {
                    cout << u8"\n=======================追加情報=======================" << endl;
                    cout << u8" 漆黒の闇ネロ（10章-5節）と同じ部位、防御力をもちます。" << endl;
                    cout << u8"======================================================" << endl;
                    bossinfodump = 1;
                }
                break;
            default:
                language_error();
            }

            playerstatus();
            preliminarydmg_bullets(0, damagelimiter);

            cout << "----------------------------------------------------------" << endl;
            cout << stockphrases_hitboxes[language][0] << endl;

            switch (language)
            {
            case 0: //English
                cout << "\n (3.0) *Critical* Head" << endl;    //60. Max damage: 9999.
                break;
            case 1: //Japanese
                cout << u8"\n (3.0) *クリティカル* 頭部" << endl;    //60. Max damage: 9999.
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 60 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();

            switch (language)
            {
            case 0: //English
                cout << "\n (1.0) Remainder of body" << endl;    //20. Max damage: 9999.
                break;
            case 1: //Japanese
                cout << u8"\n (1.0) 体の他の部分。" << endl;    //20. Max damage: 9999.
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 20 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();

        }


        else if (selectedboss == 19) //Weiss the Immaculate   (Ch11-5)  HP 30000
        {
            damagelimiter = 2;

            //Info about Weiss the Immaculate, Ch11-5 (Beginnings)
            switch (language)
            {
            case 0: //English
                cout << "\n---WEISS THE IMMACULATE--- [Ch11-5]" << endl;
                cout << " HP 30000" << endl;

                if (bossinfodump == 0)
                {
                    cout << "\n********************ADDITIONAL INFO********************" << endl;
                    cout << " This battle is unwinnable. The graphical HP bar will never deplete" << endl;
                    cout << " but damage can still be applied. Weiss's HP will never go below 1," << endl;
                    cout << " but the only consequence of changing the value to 0 is that attacks" << endl;
                    cout << " no longer have an effect. There is no hidden win condition." << endl;

                    cout << "\n Weiss the Immaculate has a DEF stat of 80. On top of that," << endl;
                    cout << " all damage types are reduced to 1/4 even before the hitbox-specific" << endl;
                    cout << " defense modifiers have been taken into account." << endl;
                    cout << "*******************************************************" << endl;
                    bossinfodump = 1;
                }
                break;
            case 1: //Japanese
                cout << u8"\n 純白の帝王ヴァイス (11章-5節)" << endl;
                cout << u8" HP 30000" << endl;

                if (bossinfodump == 0)
                {
                    cout << u8"\n=======================追加情報=======================" << endl;
                    cout << u8" この戦いに勝つことはできません。 HP ゲージは減少しませんが、" << endl;
                    cout << u8" ダメージを与えることはできます。 ヴァイスのHPが1以下になることはありません。" << endl;
                    cout << u8" 純白の帝王ヴァイスのDEF値は 80 です。 " << endl;
                    cout << u8" さらに、すべてのダメージが1/4に減少します。" << endl;
                    cout << u8"======================================================" << endl;
                    bossinfodump = 1;
                }
                break;
            default:
                language_error();
            }

            playerstatus();
            preliminarydmg_bullets(80, damagelimiter); //All damage types are reduced to 1/4th.

            cout << "----------------------------------------------------------" << endl;
            cout << stockphrases_hitboxes[language][0] << endl;

            switch (language)
            {
            case 0: //English
                cout << "\n (1.0) All of Weiss's hitboxes" << endl;    //20. Max damage: 9999.
                break;
            case 1: //Japanese
                cout << u8"\n (1.0) すべての部位" << endl;    //20. Max damage: 9999.
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 20 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();
        }


        else if (selectedboss == 20) //Weiss Empowered        (Ch11-6)  HP 16000
        {

            damagelimiter = 1;

            //Info about Weiss Empowered, Ch11-6 (Beginnings)
            switch (language)
            {
            case 0: //English
                cout << "\n---WEISS EMPOWERED--- [Ch11-6]" << endl;
                cout << " HP 16000" << endl;

                if (bossinfodump == 0)
                {
                    cout << "\n********************ADDITIONAL INFO********************" << endl;
                    cout << " Bullet damage is halved after enemy's DEF stat has been checked" << endl;
                    cout << " but before the hitbox-specific defense modifiers are applied." << endl;
                    cout << " Weiss Empowered has a DEF stat of 0." << endl;
                    cout << "*******************************************************" << endl;
                    bossinfodump = 1;
                }
                break;
            case 1: //Japanese
                cout << u8"\n オメガの力宿りしヴァイス (11章-6節)" << endl;
                cout << u8" HP 16000" << endl;

                if (bossinfodump == 0)
                {
                    cout << u8"\n=======================追加情報=======================" << endl;
                    cout << u8" 銃撃ダメージは敵のDEF値を参照した後、" << endl;
                    cout << u8" 部位ごとの防御修正値を計算・適用する以前に半減されます。" << endl;
                    cout << u8" オメガの力宿りしヴァイスのDEF値は 0 です。" << endl;
                    cout << u8"======================================================" << endl;
                    bossinfodump = 1;
                }
                break;
            default:
                language_error();
            }

            playerstatus();
            preliminarydmg_bullets(0, damagelimiter); //bullet damage is halved. Magic and melee unaffected.

            cout << "----------------------------------------------------------" << endl;
            cout << stockphrases_hitboxes[language][0] << endl;

            switch (language)
            {
            case 0: //English
                cout << "\n (2.0) *Critical* Head" << endl;    //40. Max damage: 9999.
                break;
            case 1: //Japanese
                cout << u8"\n (2.0) *クリティカル* 頭部" << endl;    //40. Max damage: 9999.
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 40 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();

            switch (language)
            {
            case 0: //English
                cout << "\n (1.0) Remainder of body" << endl;    //20. Max damage: 9999.
                break;
            case 1: //Japanese
                cout << u8"\n (1.0) 体の他の部分。" << endl;    //20. Max damage: 9999.
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 20 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();
        }


        else if (selectedboss == 21) //Crystal Feelers        (Ch12-2)  HP 0
        {

            //Info about Crystal Feelers
            switch (language)
            {
            case 0: //English
                cout << "\n---CRYSTAL FEELERS--- [Ch12-2]" << endl;
                cout << "\n The only way to defeat a Crystal Feeler is to strike with melee attack" << endl;
                cout << " while the core is exposed. The only requirement is a successful strike." << endl;
                cout << " The power level of the melee means nothing." << endl;
                cout << " Destroy all 6 and face Omega Cocoon." << endl;

                cout << "\n The HP bar on the upper right part of the screen represents 3000 HP," << endl;
                cout << " with 500 HP being depleted per Crystal Feeler that gets destroyed." << endl;
                cout << " No actual target HP is being depleted however and so the Crystal Feelers" << endl;
                cout << " can be said to have 0 HP each. Bullets and magic have no effect." << endl;

                cout << " \n As such, the Crystal Feelers are only included in this calculator" << endl;
                cout << " for the sake of covering every target that is associated with an" << endl;
                cout << " on-screen HP bar." << endl;
                break;
            case 1: //Japanese
                cout << u8" クリスタルフィーラー (12章-2節)" << endl;
                cout << u8" クリスタルフィーラーを倒す唯一の方法は、コアの露出中に近接攻撃することです。 " << endl;
                cout << u8" 攻撃に成功すれば、一撃で倒すことができます。" << endl;
                cout << u8" クリスタルフィーラーを6つすべて破壊すると、オメガコクーンとの戦闘が始まります。" << endl;
                cout << u8" 画面の右上隅にある HP バーは 3000 HP を表し、" << endl;
                cout << u8" クリスタルフィーラーが破壊されるたびに HP が 500 減少します。" << endl;
                cout << u8" ただし、実際にはクリスタルフィーラーの HP は減っていないため、" << endl;
                cout << u8" 各クリスタル フィーラーの HP は 0 であると言えます。 銃撃も魔法も効きません。" << endl;
                break;
            default:
                language_error();
            }
        }


        else if (selectedboss == 22) //Omega Cocoon           (Ch12-2)  HP 15001
        {

            damagelimiter = 2;

            //Info about Omega Cocoon, Ch12-2 (A Finale Chaotic)
            switch (language)
            {
            case 0: //English
                cout << "\n---OMEGA COCOON--- [Ch12-2]" << endl;
                cout << " HP 15001" << endl;

                if (bossinfodump == 0)
                {
                    cout << "\n********************ADDITIONAL INFO********************" << endl;
                    cout << " The full HP bar represents 20000 HP but the battle will end if" << endl;
                    cout << " simply more than 15000 HP has been depleted. Ergo, its actual" << endl;
                    cout << " health can be written as 15001." << endl;

                    cout << "\n The target has a DEF stat of 128 but the greatest defense" << endl;
                    cout << " comes from bullet damage being reduced to 25% of normal." << endl;
                    cout << " This is the same multiplier that gives Weiss Immaculate his" << endl;
                    cout << " massive defense." << endl;

                    cout << "\n Damage output from Death Penalty bullets are nerfed by" << endl;
                    cout << " both the DEF stat and the reduction to 25% damage." << endl;
                    cout << " Death Penalty shots are unaffected by the hitbox-specific" << endl;
                    cout << " damage modifier below." << endl;
                    cout << "*******************************************************" << endl;
                    bossinfodump = 1;
                }
                break;
            case 1: //Japanese
                cout << u8"\n オメガコクーン (12章-2節)" << endl;
                cout << u8" HP 15001" << endl;

                if (bossinfodump == 0)
                {
                    cout << u8"\n=======================追加情報=======================" << endl;
                    cout << u8" 最大 HP は 20000 ですが、15000 以上ダメージを与えると戦闘が終了します。 " << endl;
                    cout << u8" したがって、実質的にオメガコクーンの HP は 15001 であると言えます。" << endl;
                    cout << u8" 対象のDEF値は128ですが、" << endl;
                    cout << u8" 最大の防御は銃弾のダメージを通常の1/4になることです。" << endl;
                    cout << u8"======================================================" << endl;
                    bossinfodump = 1;
                }
                break;
            default:
                language_error();
            }



            playerstatus();
            preliminarydmg_bullets(128, damagelimiter); //Bullet-type damage is reduced to 1/4th. Magic is unaffected. Melee always deals 0 damage.

            cout << "----------------------------------------------------------" << endl;
            cout << stockphrases_hitboxes[language][0] << endl;

            switch (language)
            {
            case 0: //English
                cout << "\n (1.0) Cocoon" << endl;    //20. Max damage: 9999.
                break;
            case 1: //Japanese
                cout << u8"\n (1.0) コクーン" << endl;    //20. Max damage: 9999.
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 20 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();
        }


        else if (selectedboss == 23) //Omega Weiss            (Ch12-2)  HP 240000
        {
            damagelimiter = 2;
            //Weiss himself is separate and has same hitbox as Weiss Empowered.

            //Info about Omega Weiss, Ch12-2 (A Finale Chaotic)
            switch (language)
            {
            case 0: //English
                cout << "\n---Omega Weiss--- [Ch12-2]" << endl;
                cout << " HP 240000" << endl;

                if (bossinfodump == 0)
                {
                    cout << "\n********************ADDITIONAL INFO********************" << endl;
                    cout << " Bullet damage is reduced to 1/4th on Omega Weapon. This affects" << endl;
                    cout << " Death Penalty bullets as well." << endl;

                    cout << "\n The human Weiss body is separate from Omega Weapon and" << endl;
                    cout << " has the same hitbox data as Weiss Empowered (Ch11-6)," << endl;
                    cout << " including having bullet damage reduced by half." << endl;
                    cout << "*******************************************************" << endl;
                    bossinfodump = 1;
                }
                break;
            case 1: //Japanese
                cout << u8"\n オメガヴァイス (12章-2節)" << endl;
                cout << u8" HP 240000" << endl;

                if (bossinfodump == 0)
                {
                    cout << u8"\n=======================追加情報=======================" << endl;
                    cout << u8" オメガヴァイスに与える銃撃ダメージは 1/4 に軽減されます。" << endl;
                    cout << u8" これはデスペナルティの銃撃にも影響します。" << endl;
                    cout << u8"\n 人間体のヴァイスはオメガウェポンとは別個体で、銃弾ダメージが半分になる点を含めて、" << endl;
                    cout << u8" オメガの力宿りしヴァイス（11章-6節）と同じ部位データを持っています。" << endl;
                    cout << u8"======================================================" << endl;
                    bossinfodump = 1;
                }
                break;
            default:
                language_error();
            }


            playerstatus();
            preliminarydmg_bullets(0, damagelimiter); //Bullet-type damage is reduced to 1/4th. Magic and melee is unaffected.
            //Thunder never works on Omega Weiss.

    //Omega Weiss (Omega Weapon)
            cout << "----------------------------------------------------------" << endl;
            cout << stockphrases_hitboxes[language][0] << endl;

            switch (language)
            {
            case 0: //English
                cout << "\n* OMEGA WEISS (Planet Weapon) *" << endl;
                break;
            case 1: //Japanese
                cout << u8"\n* オメガヴァイス［星のウェポン］ *" << endl;
                break;
            default:
                language_error();
            }

            switch (language)
            {
            case 0: //English
                cout << "\n (1.2) *Critical* Blue circle in center of chest" << endl;    //24. Max damage: 9999.
                break;
            case 1: //Japanese
                cout << u8"\n (1.2) *クリティカル* 胸の真ん中にある青い円" << endl;    //24. Max damage: 9999.
                break;
            default:
                language_error();
            }
            //temporarystorage = (preliminarydamage+0.50) * 24 * hitboxmodifier.hitboxfloat;
            temporarystorage = (preliminarydamage + 0.50) * (float)1.19999992847;
            //^Wrote float literal here to solve edge-case where truncation normally rounded up instead of down.
            showhitboxdamage();


            switch (language)
            {
            case 0: //English
                cout << "\n (1.1) *Critical* Head & shoulder crystal spikes" << endl;    //22. Max damage: 9999.
                break;
            case 1: //Japanese
                cout << u8"\n (1.1) *クリティカル* 頭と肩のクリスタル スパイク" << endl;    //22. Max damage: 9999.
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 22 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();

            switch (language)
            {
            case 0: //English
                cout << "\n (0.3) Neck, torso & bottom body" << endl;    //6. Max damage: 2999.
                break;
            case 1: //Japanese
                cout << u8"\n (0.3) 首、胴体、下半身" << endl;    //6. Max damage: 2999.
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 6 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();

            switch (language)
            {
            case 0: //English
                cout << "\n (0.2) Arms" << endl;    //4. Max damage: 1999.
                break;
            case 1: //Japanese
                cout << u8"\n (0.2) 腕" << endl;    //4. Max damage: 1999.
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 4 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();

            switch (language)
            {
            case 0: //English
                cout << "\n (0.05) Floating shields" << endl;    //1. Max damage: 499.
                break;
            case 1: //Japanese
                cout << u8"\n (0.05) 浮遊シールド" << endl;    //1. Max damage: 499.
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 1 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();

            //Omega Weiss (Human Weiss)
            cout << "----------" << endl;

            switch (language)
            {
            case 0: //English
                cout << "*OMEGA WEISS (Human)*\n" << endl;
                break;
            case 1: //Japanese
                cout << u8"* オメガヴァイス［人間］ *\n" << endl;
                break;
            default:
                language_error();
            }

            preliminarydmg_bullets(0, 1); //bullet damage is halved. Magic and melee unaffected.

            switch (language)
            {
            case 0: //English
                cout << "\n (2.0) *Critical* Head" << endl;    //40. Max damage: 9999.
                break;
            case 1: //Japanese
                cout << u8"\n (2.0) *クリティカル* 頭部" << endl;    //40. Max damage: 9999.
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 40 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();

            switch (language)
            {
            case 0: //English
                cout << "\n (1.0) Remainder of body" << endl;    //20. Max damage: 9999.
                break;
            case 1: //Japanese
                cout << u8"\n (1.0) 体の他の部分。" << endl;    //20. Max damage: 9999.
                break;
            default:
                language_error();
            }
            temporarystorage = (preliminarydamage + 0.50) * 20 * hitboxmodifier.hitboxfloat;
            showhitboxdamage();
        }

        return 0;
    }


    //Current "Player Status": Level, Equipment, Magic and Chain.
    int playerstatus()
    {

        switch (language)
        {
        case 0: //English
            //cout << "\n----------------------------------------------------------" << endl;
            cout << "\n==========================PLAYER STATUS==========================" << endl;
            cout << "Difficulty Mode: " << difficultymodes[language][difficultymode] << endl;
            cout << "Vincent's stats: " << flush;
            cout << "Lv " << vincentlevel << flush;
            cout << " | Max HP " << vincentHP[vincentlevel] << flush;
            cout << " | STR " << vincentSTR[vincentlevel] << flush;
            cout << " | DEF " << vincentDEF[vincentlevel] << flush;
            cout << " | DEX " << vincentDEX[vincentlevel] << flush;
            cout << " | INT " << vincentINT[vincentlevel] << endl;

            if (bullet_type == 0) //Handgun
                cout << "Equipment: " << handguns[language][handgun] << " + " << barrels[language][barrel] << " + " << powerboosters[language][powerbooster] << endl;
            else if (bullet_type == 1) //Machine gun
                cout << "Equipment: " << machineguns[language][machinegun] << " + " << barrels[language][barrel] << " + " << powerboosters[language][powerbooster] << endl;
            else if (bullet_type == 2) //Rifle
                cout << "Equipment: " << rifles[language][rifle] << " + " << barrels[language][barrel] << " + " << powerboosters[language][powerbooster] << endl;
            else
                cout << "" << endl;
            cout << "Magic Settings: " << manaboosters[language][manabooster] << " | " << "Enemy Magic Defense: " << enemymagicdefense[language][magicdamagelimiter] << endl;
            cout << "Killchain: " << killchain << endl;
            break;
        case 1: //Japanese
            cout << u8"\n==========================プレイヤーステータス==========================" << endl;
            cout << u8"難易度： " << difficultymodes[language][difficultymode] << endl;
            cout << u8"ヴィンセントの統計： " << flush;
            cout << u8"レベル [Lv] " << vincentlevel << flush;
            cout << u8" | 最大HP " << vincentHP[vincentlevel] << flush;
            cout << u8" | STR " << vincentSTR[vincentlevel] << flush;
            cout << u8" | DEF " << vincentDEF[vincentlevel] << flush;
            cout << u8" | DEX " << vincentDEX[vincentlevel] << flush;
            cout << u8" | INT " << vincentINT[vincentlevel] << endl;

            if (bullet_type == 0) //Handgun
                cout << u8"器材： " << handguns[language][handgun] << " + " << barrels[language][barrel] << " + " << powerboosters[language][powerbooster] << endl;
            else if (bullet_type == 1) //Machine gun
                cout << u8"器材： " << machineguns[language][machinegun] << " + " << barrels[language][barrel] << " + " << powerboosters[language][powerbooster] << endl;
            else if (bullet_type == 2) //Rifle
                cout << u8"器材： " << rifles[language][rifle] << " + " << barrels[language][barrel] << " + " << powerboosters[language][powerbooster] << endl;
            else
                cout << "" << endl;
            cout << u8"魔法の設定： " << manaboosters[language][manabooster] << " | " << u8"敵の魔法防御： " << enemymagicdefense[language][magicdamagelimiter] << endl;
            cout << u8"チェイン： " << killchain << endl;
            break;
        default:
            language_error();
        }

        return 0;
    }


    int saveplayerpreset() {


        switch (language)
        {
        case 0: //English
            cout << "Choose a name for the player preset. The extension [.savefile] is added automatically." << endl;
            break;
        case 1: //Japanese
            cout << u8"プレーヤー プリセットの名前を選択します。 拡張子 [.savefile] が自動的に付加されます。" << endl;
            break;
        default:
            language_error();
        }

        cin >> playerfile_save;
        filename_full = playerfile_save + ".savefile";


        ofstream write_preset;
        write_preset.open(filename_full, ios::out | ios::binary);

        char* memblock;
        memblock = new char[13];

        memblock[0] = difficultymode;
        memblock[1] = bullet_type;
        memblock[2] = vincentlevel;
        memblock[3] = handgun;
        memblock[4] = machinegun;
        memblock[5] = rifle;
        memblock[6] = barrel;
        memblock[7] = powerbooster;
        memblock[8] = manabooster;
        memblock[9] = killchain;
        memblock[10] = magicdamagelimiter;
        memblock[11] = menu_default;
        memblock[12] = selectedboss;

        write_preset.write(memblock, 13);

        write_preset.close();
        delete[] memblock;

        switch (language)
        {
        case 0: //English
            cout << "The player preset [" << playerfile_save << ".savefile] has now been created." << endl;
            break;
        case 1: //Japanese
            cout << u8"プレイヤープリセット「" << playerfile_save << u8".savefile」を作成しました。" << endl;
            break;
        default:
            language_error();
        }
        return 0;
    }


    int loadplayerpreset() {


        switch (language)
        {
        case 0: //English
            cout << "Type the name of your player preset, minus the [.savefile] extension. Be aware of uppercase and lowercase." << endl;
            break;
        case 1: //Japanese
            cout << u8"「.savefile」拡張子を付けずにプレーヤー プリセットの名前を入力します。" << endl;
            break;
        default:
            language_error();
        }

        cin >> playerfile_load;

        filename_full = playerfile_load + ".savefile";

        ifstream read_preset;
        read_preset.open(filename_full, ios::in | ios::binary);
        char* memblock;
        memblock = new char[13];
        read_preset.read(memblock, 13);

        difficultymode = memblock[0];
        bullet_type = memblock[1];
        vincentlevel = memblock[2];
        vincentHP[vincentlevel];
        vincentSTR[vincentlevel];
        vincentDEF[vincentlevel];
        vincentDEX[vincentlevel];
        vincentINT[vincentlevel];
        handgun = memblock[3];
        handgunpowervalues[handgun];
        machinegun = memblock[4];
        machinegunpowervalues[machinegun];
        rifle = memblock[5];
        riflepowervalues[rifle];
        barrel = memblock[6];
        powerbooster = memblock[7];
        manabooster = memblock[8];
        killchain = memblock[9];
        if (killchain<5 && killchain>-1) //if killchain is between 0 and 4
            chaindamage = killchain * 30;
        else if (killchain > 4 && killchain < 100) //if killchain is between 5 and 99
            chaindamage = (killchain * 100) - 300;

        if (bullet_type == 0)
        {
            framepower = handgunpowervalues[handgun];
        }
        else if (bullet_type == 1)
        {
            framepower = machinegunpowervalues[machinegun];
        }
        else if (bullet_type == 2)
        {
            framepower = riflepowervalues[rifle];
        }

        barrelpower = barrelpowervalues[barrel];
        powerboostervalues[powerbooster];
        manaboostervalues[manabooster];

        magicdamagelimiter = memblock[10];
        menu_default = memblock[11];
        selectedboss = memblock[12];

        read_preset.close();
        delete[] memblock;


        switch (language)
        {
        case 0: //English
            cout << "Your player preset [" << playerfile_load << ".savefile] has now been loaded." << endl;
            break;
        case 1: //Japanese
            cout << u8"プレイヤープリセット「" << playerfile_load << u8".savefile」を読み込みました。" << endl;
            break;
        default:
            language_error();
        }

        return 0;
    }


    int mainmenu() {

        switch (language)
        {
        case 0: //English
            cout << "\nMake your selection." << endl;
            cout << " 0 - Difficulty Mode\t|  9 - Mana Booster" << endl;
            cout << " 1 - Vincent Level\t| 10 - Enemy Magic Defense" << endl;
            cout << " 2 - Killchain\t\t| 11 - Magic Damage" << endl;
            cout << " 3 - Handgun Frame\t| 12 - Magic- & MP Info" << endl;
            cout << " 4 - Machine Gun Frame\t| 13 - Boss Info" << endl;
            if (menu_default == 0)
                cout << " 5 - Rifle Frame\t| " << endl;
            else
                cout << " 5 - Rifle Frame\t| 14 - Back to top menu" << endl;
            cout << " 6 - Barrel\t\t| 15 - Save Player Preset" << endl;
            cout << " 7 - Power Booster\t| 16 - Load Player Preset" << endl;
            cout << " 8 - Boss Select\t| 17 - Change Language" << endl;

            break;
        case 1: //Japanese
            cout << u8"\n選んでください。" << endl;
            cout << u8" 0 - 難易度\t\t\t|  9 - 魔法ブーストパーツ" << endl;
            cout << u8" 1 - ヴィンセントのレベル\t| 10 - 敵の魔法防御" << endl;
            cout << u8" 2 - チェイン\t\t\t| 11 - 魔法ダメージ" << endl;
            cout << u8" 3 - ハンドガンフレーム\t\t| 12 - 魔力・MP情報| " << endl;
            cout << u8" 4 - マシンガンフレーム\t\t| 13 - ボス情報" << endl;
            if (menu_default == 0)
                cout << u8" 5 - ライフルフレーム\t\t| " << endl;
            else
                cout << u8" 5 - ライフルフレーム\t\t| 14 - トップメニューに戻ります。" << endl;
            cout << u8" 6 - バレル\t\t\t| 15 - プレーヤープリセットを保存する" << endl;
            cout << u8" 7 - パワーブースター\t\t| 16 - プレイヤープリセットをロードする" << endl;
            cout << u8" 8 - ボス選択\t\t\t| 17 - 既定の言語を設定する。" << endl;
            break;
        default:
            language_error();
        }

        return 0;
    }


    //Actual selection tree in main menu.
    int selectiontree()
    {

    redoselection:
        cout << stockphrases_selection[language][0] << flush;
        cin >> selection;

        if (selection == 0) //Difficulty Mode
        {
            difficultymode_select();
        }
        else if (selection == 1) //Vincent Level
        {
            vincentlv_select();
        }
        else if (selection == 2) //Killchain
        {
            chain_select();
        }
        else if (selection == 3) //Handgun Frame
        {
            bullet_type = 0;
            handgun_select();
        }
        else if (selection == 4) //Machine Gun Frame
        {
            bullet_type = 1;
            machinegun_select();
        }
        else if (selection == 5) //Rifle Frame
        {
            bullet_type = 2;
            rifle_select();
        }
        else if (selection == 6) //Barrel
        {
            barrel_select();
        }
        else if (selection == 7) //Power Booster
        {
            powerbooster_select();
        }
        else if (selection == 8) //Boss Select. Leads to damage application.
        {
            menu_default = 1;
            selectboss_bulletdmg(); //Eventually leads to boss_selected() call inside main().
        }
        else if (selection == 9) //Mana Booster
        {
            manabooster_select();
        }
        else if (selection == 10) //Enemy Magic Defense
        {
            magicdefense_select();
        }
        else if (selection == 11) //Magic Damage. Leads to damage application.
        {
            menu_default = 2; //After return, leads to magicdmg() call inside main().
        }
        else if (selection == 12) //Magic- & MP Info
        {
            magicandmp_info();
        }
        else if (selection == 13) //Boss Info
        {
            boss_info();
        }
        else if (selection == 14) //14 - Back to top level of main menu
        {
            if (menu_default == 0) //Can't return to top menu if you are already there.
            {
                cout << stockphrases_error[language][0] << endl;
                goto redoselection;
            }
            else
                menu_default = 0;
        }
        else if (selection == 15) //Save Player Preset
        {
            saveplayerpreset();
        }
        else if (selection == 16) //Load Player Preset
        {
            loadplayerpreset();
        }
        else if (selection == 17) //Change Language
        {
            select_language();
        }
        else
        {
            cout << stockphrases_error[language][0] << endl;
            goto redoselection;
        }

        return 0;
    }


    int playerstatus_programbootup() {

        difficultymode = 0; //Normal/Hard mode
        bullet_type = 0; //Handgun
        vincentlevel = 1;
        handgun = 4; //Cerberus
        barrel = 7; //Long Barrel
        powerbooster = 0; //No Power Booster
        manabooster = 0; //No Mana Booster
        killchain = 0;

        //Cementing the actual power values.
        difficultymode_attackmodifier[difficultymode];
        chaindamage = 0;
        framepower = handgunpowervalues[handgun];
        barrelpower = barrelpowervalues[barrel];
        powerboostervalues[powerbooster];
        manaboostervalues[manabooster];

        vincentHP[1];
        vincentSTR[1];
        vincentDEF[1];
        vincentDEX[1];
        vincentINT[1];

        damagelimiter = 0; //0 = 1.0. 
        magicdamagelimiter = 0;

        menu_default = 0;

        return 0;
    }

};
//End of struct "calculations_postoriginal"



int main()
{
    UTF8CodePage use_utf8;
    default_settings settings;
    settings.controlconfigfile();

    switch (language)
    {
    case 0: //English
        cout << "\n Welcome to the Dirge of Cerberus (DoC) FFVII Damage Calculator!" << endl;
        cout << "       [Version 1.1] Written by Shademp." << endl;
        cout << " Use this to calculate bullet- and magic damage on boss enemies in story mode." << endl;
        cout << "\n This tool is not intended for use with the original Japanese release of DoC.\n" << endl;
        break;

    case 1: //Japanese
        cout << u8"\n ダージュ オブ ケルベロス - FFVII - ダメージ計算機へようこそ！" << endl;
        cout << u8"   [バージョン1.1] Shademp によって書かれたプログラム。英語から日本語への自動翻訳は、" << endl;
        cout << u8"    littletonbi(ことんび)によって制御および修正されました。" << endl;
        cout << u8" ストーリー モードでボスに与える銃撃と魔法のダメージを計算することができます。" << endl;
        cout << u8"\n このツールは、海外通常版および日本インターナショナル版での使用を想定したものです。" << endl;
        cout << u8"日本初期オリジナル版DoCでの使用を想定したものではありません。" << endl;
        break;
    default:
        settings.language_error();
    }

    calculations_postoriginal journey;
    journey.playerstatus_programbootup();

home:
    if (menu_default == 0) //Return to top menu.
    {
        journey.playerstatus();
        journey.mainmenu();
        journey.selectiontree();
    }
    else if (menu_default == 1) //If inside boss damage calculations.
    {
        journey.boss_selected();
        journey.mainmenu();
        journey.selectiontree();
    }
    else if (menu_default == 2) //If inside magic damage calculations.
    {
        journey.magicdmg();
        journey.playerstatus();
        journey.mainmenu();
        journey.selectiontree();
    }
    goto home;


    return 0;
}
