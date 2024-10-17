#include <iostream>
#include <vector>
#include <stdexcept>
#include <format>
#include <map>

struct item_entry {
	bool nabidka;
	int id;
	int price;
	int poradi;
};

int countDifferentIds(std::vector<item_entry> entry_vec) {
	std::map<int, int> different_ids;

	for (int i = 0; i < (int)entry_vec.size(); i++) {
		different_ids[entry_vec[i].id] += 1;
	}

	return (int)different_ids.size();
}

int getLowestItemId(std::vector<item_entry> entry_vec, int max_id) {
	int lowest = max_id;

	for (int i = 0; i < (int)entry_vec.size(); i++) {
		if (entry_vec[i].id < lowest) {
			lowest = entry_vec[i].id;
		}
	}

	return lowest;
}

int getProfit(std::pair<item_entry, item_entry> transaction) {
	item_entry nabidka = transaction.first;
	item_entry poptavka = transaction.second;
	return poptavka.price - nabidka.price;
}

int main(int argc, char *argv[]) {
	int entry_count;
	std::vector<item_entry> entry_vec;

	// Read input
	std::cin >> entry_count;

	for (int i = 0; i < entry_count; i++) {
		item_entry vstup;
		
		char znak;
		std::cin >> znak >> vstup.id >> vstup.price;

		if (znak == 'N') {
			vstup.nabidka = true;
		} else if (znak == 'P') {
			vstup.nabidka = false;
		} else {
			throw std::logic_error(std::format("Znak vstupu neni 'N' nebo 'P' ale {}", znak));
		}

		vstup.poradi = i;

		entry_vec.push_back(vstup);
	}

	const int MIN_ID = getLowestItemId(entry_vec, entry_count);
	const int MAX_ID = entry_count;

	std::cout << "MIN_ID = " << MIN_ID << std::endl;
	std::cout << "MAX_ID = " << MAX_ID << std::endl; 

	// Najdeme nejvyhodnejsi nabidku a poptavku pro kazdou entry
	std::map<int, item_entry> nejvyhodnejsi_nabidky;
	std::map<int, item_entry> nejvyhodnejsi_poptavky;
	std::vector<item_entry> nabidky;
	std::vector<item_entry> poptavky;

	// Pro kazdy item najdeme nejvyhodnejsi nabidku a poptavku
	for (int item_id = MIN_ID; item_id <= MAX_ID; item_id++) {
		for (int i = 0; i < (int)entry_vec.size(); i++) {
			item_entry item = entry_vec[i];

			if (item_id != item.id) {
				continue;
			}

			if (item.nabidka) {
				// Nabidka
				nabidky.push_back(item);

				if (nejvyhodnejsi_nabidky[item_id].id == 0) {
					nejvyhodnejsi_nabidky[item_id] = item;
				} else if (item.price < nejvyhodnejsi_nabidky[item_id].price) {
					nejvyhodnejsi_nabidky[item_id] = item;
				}

			} else {
				// Poptavka
				poptavky.push_back(item);

				if (nejvyhodnejsi_poptavky[item_id].id == 0) {
					nejvyhodnejsi_poptavky[item_id] = item;
				} else if (item.price > nejvyhodnejsi_poptavky[item_id].price) {
					nejvyhodnejsi_poptavky[item_id] = item;
				}
			}
		}
	}

	std::cout << "Nabidky:" << std::endl;
	for (int i = 0; i < (int)nabidky.size(); i++) {
		std::cout << std::format("id {}; price {}; poradi {}", nabidky[i].id, nabidky[i].price, nabidky[i].poradi) << "\n";
	}

	std::cout << "Poptavky:" << std::endl;
	for (int i = 0; i < (int)poptavky.size(); i++) {
		std::cout << std::format("id {}; price {}; poradi {}", poptavky[i].id, poptavky[i].price, poptavky[i].poradi) << "\n";
	}

	std::cout << "Nejvyhodnejsi nabidky:" << std::endl;
	for(const auto& elem : nejvyhodnejsi_nabidky) {
		std::cout << std::format("id {}; price {}; poradi {}", elem.second.id, elem.second.price, elem.second.poradi) << "\n";
	}

	std::cout << "Nejvyhodnejsi poptavky:" << std::endl;
	for(const auto& elem : nejvyhodnejsi_poptavky) {
		std::cout << std::format("id {}; price {}; poradi {}", elem.second.id, elem.second.price, elem.second.poradi) << "\n";
	}

	std::vector<std::pair<item_entry, item_entry>> mozne_transakce;

	for (int item_id = MIN_ID; item_id <= MAX_ID; item_id++) {
		
		if (nejvyhodnejsi_nabidky[item_id].id != 0 and nejvyhodnejsi_poptavky[item_id].id != 0 and // Mame par pro transakci??
			nejvyhodnejsi_nabidky[item_id].poradi < nejvyhodnejsi_poptavky[item_id].poradi) { // Byla nabidka polozena drive nez poptavka
			item_entry nabidka = nejvyhodnejsi_nabidky[item_id];
			item_entry poptavka = nejvyhodnejsi_poptavky[item_id];
			mozne_transakce.push_back(std::pair<item_entry, item_entry>(nabidka, poptavka));
		}
	}

	std::pair<item_entry, item_entry> nejvyhodnejsi_transakce;
	for (int i = 0; i < (int)mozne_transakce.size(); i++) {
		std::pair<item_entry, item_entry> transakce = mozne_transakce[i];
		item_entry nabidka = transakce.first;
		item_entry poptavka = transakce.second;
		int zisk = poptavka.price - nabidka.price;

		std::cout << std::format("--- Nabidka {} ---", i) << std::endl;
		std::cout << std::format("Nabidka id {}; cena {}; poradi {}", nabidka.id, nabidka.price, nabidka.poradi) << std::endl;
		std::cout << std::format("Poptavka id {}; cena {}; poradi {}", poptavka.id, poptavka.price, poptavka.poradi) << std::endl;
		std::cout << std::format("Mozny zisk {}", zisk) << std::endl;

		if (zisk > getProfit(nejvyhodnejsi_transakce)) {
			nejvyhodnejsi_transakce = transakce;
		}
	}

	std::cout << "--- Nejvyhodnejsi transakce ---" << std::endl;
	std::cout << std::format("Nabidka: id {}; cena {}; poradi {}", nejvyhodnejsi_transakce.first.id,
										nejvyhodnejsi_transakce.first.price, nejvyhodnejsi_transakce.first.poradi) << std::endl;
	std::cout << std::format("Poptavka: id {}; cena {}; poradi {}", nejvyhodnejsi_transakce.second.id,
										nejvyhodnejsi_transakce.second.price, nejvyhodnejsi_transakce.second.poradi) << std::endl;
	std::cout << std::format("Zisk transakce {}", getProfit(nejvyhodnejsi_transakce)) << std::endl;

	return 0;
}
