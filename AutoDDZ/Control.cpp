#include "Control.h"
bool CreateRandomCardSuit(int suit_count) {
	std::random_device rd;
	std::mt19937 g(rd());
	array<int, 54> array_cards;
	while (suit_count--) {
		iota(array_cards.begin(), array_cards.end(), 1);
		shu
	}
}