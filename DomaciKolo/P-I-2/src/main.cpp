#include <iostream>
#include <vector>
#include <stdexcept>
// #include <format>
#include <map>
#include <algorithm>

struct ItemEntry {
	bool nabidka;
	int id;
	int price;
	int poradi;
};

class Transakce {
	public:
	ItemEntry nabidka;
	ItemEntry poptavka;

	Transakce(ItemEntry nabidka, ItemEntry poptavka) {
		if (not nabidka.nabidka) {
			throw std::logic_error("nabidka neni nabidka");
		}

		if (poptavka.nabidka) {
			throw std::logic_error("poptavka neni poptavka");
		}

		this->nabidka = nabidka;
		this->poptavka = poptavka;
	}

	int getProfit() {
		return this->poptavka.price - this->nabidka.price;
	}
};

int compareTransactionProfit(Transakce a, Transakce b) {
	return a.getProfit() > b.getProfit();
}

int getProfit(std::pair<ItemEntry, ItemEntry> transaction) {
	ItemEntry nabidka = transaction.first;
	ItemEntry poptavka = transaction.second;
	return poptavka.price - nabidka.price;
}

int main(int argc, char *argv[]) {
	int entry_count;
	std::vector<ItemEntry> nabidky;
	std::vector<ItemEntry> poptavky;

	// Read input
	std::cin >> entry_count;

	for (int i = 0; i < entry_count; i++) {
		ItemEntry item;
		
		char znak;
		std::cin >> znak >> item.id >> item.price;

		if (znak == 'N') {
			item.nabidka = true;
		} else if (znak == 'P') {
			item.nabidka = false;
		} else {
			throw std::logic_error("Znak vstupu neni 'N' nebo 'P'");
		}

		item.poradi = i;

		if (item.nabidka) {
			nabidky.push_back(item);
		} else {
			poptavky.push_back(item);
		}
	}

	std::vector<Transakce> mozne_transakce;
	
	// Pro kazdou nabidku najdeme poptavku a pokusime se vytvorit transakci
	for (int index_nabidky = 0; index_nabidky < (int)nabidky.size(); index_nabidky++) {
		ItemEntry nabidka = nabidky[index_nabidky];

		for (int index_poptavky = 0; index_poptavky < (int)poptavky.size(); index_poptavky++) {
			ItemEntry poptavka = poptavky[index_poptavky];

			// Pokud nemáme stejné předměty v nabídce a poptávce nebo se nabídka objevila později než poptávka
			// Tak přeskočíme pár
			if ((nabidka.id != poptavka.id) || (nabidka.poradi > poptavka.poradi)) {
				continue;
			}

			Transakce mozna_transakce = Transakce(nabidka, poptavka);

			// Pokud není možný profit tak přeskočíme
			if (mozna_transakce.getProfit() < 1) {
				continue;
			}

			mozne_transakce.push_back(mozna_transakce);
		}
	}

	// std::cout << "Nabídky:" << std::endl;
	// for (int i = 0; i < (int)nabidky.size(); i++) {
	// 	std::cout << std::format("id {}; price {}; pořadí {}", nabidky[i].id, nabidky[i].price, nabidky[i].poradi) << "\n";
	// }

	// std::cout << "Poptávky:" << std::endl;
	// for (int i = 0; i < (int)poptavky.size(); i++) {
	// 	std::cout << std::format("id {}; price {}; pořadí {}", poptavky[i].id, poptavky[i].price, poptavky[i].poradi) << "\n";
	// }

	// std::cout << "Možné transakce:" << std::endl;
	// for(int i = 0; i < (int)mozne_transakce.size(); i++) {
	// 	Transakce transakce = mozne_transakce[i];
	// 	std::cout << std::format("nabídka {}; poptávka {}; profit {}", transakce.nabidka.poradi, transakce.poptavka.poradi, transakce.getProfit()) << "\n";
	// }

	// Seřadíme možné transakce podle výše profitu
	std::sort(mozne_transakce.begin(), mozne_transakce.end(), compareTransactionProfit);

	// std::cout << "Seřazené možné transakce:" << std::endl;
	// for(int i = 0; i < (int)mozne_transakce.size(); i++) {
	// 	Transakce transakce = mozne_transakce[i];
	// 	std::cout << std::format("nabídka {}; poptávka {}; profit {}", transakce.nabidka.poradi, transakce.poptavka.poradi, transakce.getProfit()) << "\n";
	// }

	// Pokusíme se maximalizovat profit pomocí provádění co nejvíce transakcí co můžeme
	std::vector<Transakce> vybrane_transakce;

	if ((int)mozne_transakce.size() > 0) {
		vybrane_transakce.push_back(mozne_transakce[0]); // Přidáme nejvýhodnější transakci
	} else {
		std::cout << 0 << std::endl;
		return 0;
	}

	// Zkontrolujeme zda lze provést další transakce
	// Začínáme na indexu 1 protože 0 už byl přidán
	for (int index_mozne_transakce = 1; index_mozne_transakce < (int)mozne_transakce.size(); index_mozne_transakce++) {
		Transakce mozna_transakce = mozne_transakce[index_mozne_transakce];
		bool valid_transakce = false;

		for (int index_vtransakce = 0; index_vtransakce < (int)vybrane_transakce.size(); index_vtransakce++) {
			Transakce vybrana_transakce = vybrane_transakce[index_vtransakce];

			// Využijeme jednoduchý algorytmus na detekci kolize dvou 1D čar
			int x1 = mozna_transakce.nabidka.poradi;
			int x2 = mozna_transakce.poptavka.poradi;
			int y1 = vybrana_transakce.nabidka.poradi;
			int y2 = vybrana_transakce.poptavka.poradi;

			if ((y1 < x1 && y1 < x2 && y2 < x1 && y2 < x2) || (y1 > x1 && y1 > x2 && y2 > x1 && y2 > x2)) {
				valid_transakce = true;
				break;
			}
		}

		if (valid_transakce == true) {
			vybrane_transakce.push_back(mozna_transakce);
		}
	}

	// std::cout << "Vybrané transakce:" << std::endl;
	int total_profit = 0;
	for(int i = 0; i < (int)vybrane_transakce.size(); i++) {
		Transakce transakce = vybrane_transakce[i];
		// std::cout << std::format("nabídka {}; poptávka {}; profit {}", transakce.nabidka.poradi, transakce.poptavka.poradi, transakce.getProfit()) << "\n";
		total_profit += transakce.getProfit();
	}

	// std::cout << "Finální profit: " << total_profit << std::endl;
	
	if (total_profit < 0) {
		total_profit = 0;
	}

	std::cout << total_profit << std::endl;

	return 0;
}
