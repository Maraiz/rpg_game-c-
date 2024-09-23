#include <iostream>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include <fstream> // Untuk file handling
#include <string>

using namespace std;

class Character {
public:
    string name;
    int health, maxHealth, mana, maxMana;
    int strength, intelligence, dexterity, vitality;
    float critChance, critDamage;
    int money;
    int level;
    int xp, xpToLevelUp;

    Character(string n, int h, int m, int str, int intel, int dex, int vit, float critC, float critD, int cash)
        : name(n), maxHealth(h), health(h), maxMana(m), mana(m),
          strength(str), intelligence(intel), dexterity(dex), vitality(vit),
          critChance(critC), critDamage(critD), money(cash), level(1), xp(0), xpToLevelUp(100) {}

    void attack(Character& target) {
        int damage = rand() % (strength + dexterity / 2) + 1;
        bool isCritical = (rand() % 100) < (critChance * 100);

        if (isCritical) {
            damage += damage * critDamage;
            cout << name << " melakukan serangan kritikal!" << endl;
        }

        // Animasi serangan
        cout << name << " menyerang " << target.name << "...\n";
        this_thread::sleep_for(chrono::milliseconds(1000)); // Tunggu 1 detik
        cout << "Serangan...!" << endl;
        this_thread::sleep_for(chrono::milliseconds(500)); // Tunggu setengah detik

        target.health -= damage;
        cout << name << " memberikan " << damage << " damage pada " << target.name << "!\n";
    }

    void useSkill(Character& target) {
        int manaCost = 20;
        if (mana >= manaCost) {
            mana -= manaCost; // Mengurangi mana
            int skillDamage = rand() % (intelligence * 2 + dexterity) + 10;

            // Animasi skill
            cout << name << " menggunakan skill khusus...\n";
            this_thread::sleep_for(chrono::milliseconds(1000)); // Tunggu 1 detik
            cout << "Skill aktif...!" << endl;
            this_thread::sleep_for(chrono::milliseconds(500)); // Tunggu setengah detik

            target.health -= skillDamage; // Mengurangi health musuh
            cout << name << " memberikan " << skillDamage << " damage pada " << target.name << "!\n";
        } else {
            cout << name << " tidak memiliki cukup mana untuk menggunakan skill!\n";
        }
    }

    bool isAlive() {
        return health > 0;
    }

    void restoreHealth() {
        health = maxHealth;  // Mengembalikan kesehatan maksimum
        mana = maxMana;      // Mengembalikan mana maksimum
    }

    void gainXP(int amount) {
        xp += amount;
        cout << name << " mendapatkan " << amount << " XP!\n";
        checkLevelUp();
    }

    void checkLevelUp() {
        while (xp >= xpToLevelUp) {
            xp -= xpToLevelUp; // Mengurangi XP yang berlebihan
            level++;
            xpToLevelUp += 50; // Meningkatkan XP yang dibutuhkan untuk level selanjutnya
            maxHealth += 10; // Meningkatkan kesehatan maksimum
            strength += 2; // Meningkatkan STR
            cout << name << " naik level! Sekarang level " << level << "!\n";
        }
    }

    void status() {
        cout << "Status " << name << ":\n";
        cout << "Level: " << level << "   XP: " << xp << "/" << xpToLevelUp << endl;
        cout << "Health: " << health << "/" << maxHealth << "   Mana: " << mana << "/" << maxMana << endl;
        cout << "STR: " << strength << endl;
        cout << "INT: " << intelligence << endl;
        cout << "DEX: " << dexterity << endl;
        cout << "VIT: " << vitality << endl;
        cout << "Critical Chance: " << critChance * 100 << "%\n";
        cout << "Critical Damage: " << critDamage * 100 << "%\n";
        cout << "Uang: " << money << " gold\n";
    }

    // Fungsi untuk menyimpan status karakter ke file
    void saveToFile() {
        ofstream file("character_save.txt");
        if (file.is_open()) {
            file << name << endl;
            file << health << " " << maxHealth << " "
                 << mana << " " << maxMana << " "
                 << strength << " " << intelligence << " "
                 << dexterity << " " << vitality << " "
                 << critChance << " " << critDamage << " "
                 << money << " " << level << " "
                 << xp << " " << xpToLevelUp << endl;
            file.close();
            cout << "Status karakter disimpan ke file!\n";
        } else {
            cout << "Gagal membuka file untuk menyimpan!\n";
        }
    }

    // Fungsi untuk memuat status karakter dari file
    void loadFromFile() {
        ifstream file("character_save.txt");
        if (file.is_open()) {
            file >> name;
            file >> health >> maxHealth >> mana >> maxMana
                 >> strength >> intelligence >> dexterity >> vitality
                 >> critChance >> critDamage >> money >> level
                 >> xp >> xpToLevelUp;
            cout << "Status karakter dimuat dari file!\n";
            file.close();
        } else {
            cout << "Gagal membuka file untuk memuat!\n";
        }
    }
};

Character generateEnemy() {
    int health = rand() % 50 + 50; // Health antara 50 - 100
    int strength = rand() % 10 + 5; // STR antara 5 - 15
    int intelligence = rand() % 10 + 5; // INT antara 5 - 15
    int dexterity = rand() % 10 + 5; // DEX antara 5 - 15
    int vitality = rand() % 10 + 5; // VIT antara 5 - 15
    return Character("Musuh", health, 30, strength, intelligence, dexterity, vitality, 0.05f, 0.25f, 50);
}

void battle(Character& player) {
    while (true) {
        Character enemy = generateEnemy();
        cout << "\nMusuh muncul: " << enemy.name << "!\n";

        while (player.isAlive() && enemy.isAlive()) {
            cout << "\n--- Giliran Pemain ---\n";
            cout << "1. Serang\n";
            cout << "2. Gunakan Skill (Mana 20)\n";
            cout << "3. Lihat Status\n";
            cout << "4. Simpan Game\n";
            cout << "Pilihanmu: ";
            int choice;
            cin >> choice;

            bool playerTurn = true; // Menandakan giliran pemain

            if (choice == 1) {
                player.attack(enemy);
            } else if (choice == 2) {
                player.useSkill(enemy);
            } else if (choice == 3) {
                player.status();
                playerTurn = false; // Setel ke false agar musuh tidak menyerang
            } else if (choice == 4) {
                player.saveToFile(); // Simpan status karakter
                continue; // Kembali ke pilihan tanpa menyerang
            } else {
                cout << "Pilihan tidak valid.\n";
                continue;
            }

            if (!enemy.isAlive()) {
                cout << enemy.name << " telah dikalahkan!\n";
                player.gainXP(50);  // Pemain mendapatkan XP setelah menang
                player.money += 50;  // Pemain mendapatkan uang setelah menang
                player.restoreHealth(); // Mengembalikan kesehatan dan mana
                break;
            }

            if (playerTurn) {
                cout << "\n--- Giliran Musuh ---\n";
                enemy.attack(player); // Musuh menyerang pemain
            }

            if (!player.isAlive()) {
                cout << player.name << " telah dikalahkan. Game Over.\n";
                return;
            }
        }
    }
}

int main() {
    srand(time(0));  // Seed untuk angka acak

    cout << "Selamat datang di Game RPG Turn-Based tanpa akhir!\n";
    cout << "Masukkan nama karaktermu: ";
    string playerName;
    cin >> playerName;

    // Membuat karakter pemain dengan status lengkap
    Character player(playerName, 150, 50, 20, 15, 10, 20, 0.1f, 0.5f, 100);  // Pemain

    // Cek apakah ada file penyimpanan
    ifstream saveFile("character_save.txt");
    if (saveFile.is_open()) {
        player.loadFromFile(); // Muat status karakter jika file ada
        saveFile.close();
    }

    battle(player);

    cout << "Terima kasih telah bermain!\n";
    return 0;
}
