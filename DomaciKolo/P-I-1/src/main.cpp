#include <iostream>
#include <vector>

struct train_rule {
	int train1_id;
	int train2_id;
	int min_delay;
};

int main(int argc, char *argv[]) {
	int train_count, rule_count;
	std::vector<train_rule> rule_vec;

	// Read from standard input
	std::cin >> train_count >> rule_count;

	for (int i = 0; i < rule_count; i++) {
		train_rule rule;
		
		std::cin >> rule.train1_id >> rule.train2_id >> rule.min_delay;

		rule_vec.push_back(rule);
	}

	
	

	return 0;
}
