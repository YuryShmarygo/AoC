#include<vector>
#include<set>

#pragma warning (disable : 4102)

std::vector<long long> Process(const std::vector<long long>& r)
{
	std::vector<long long> reg = r;
	long long last = 0, res1 = -1;
	std::set<long long> ans;
l_0: reg[3] = 123; //(0) seti 123  0  3
l_1: reg[3] = reg[3] & 456; //(1) bani  3 456  3
l_2: reg[3] = reg[3] == 72 ? 1 : 0; //(2) eqri  3 72  3
l_3: if (reg[3] == 0) goto l_4; else goto l_5; //(3) addr  3  2  2
l_4: goto l_1; //(4) seti  0  0  2
l_5: reg[3] = 0; //(5) seti  0  5  3
l_6: reg[1] = reg[3] | 65536; //(6) bori  3 65536  1
l_7: reg[3] = 10373714; //(7) seti 10373714  2  3
l_8: reg[5] = reg[1] & 255; //(8) bani  1 255  5
l_9: reg[3] = reg[3] + reg[5]; //(9) addr  3  5  3
l_10: reg[3] = reg[3] & 16777215; //(10) bani  3 16777215  3
l_11: reg[3] = reg[3] * 65899; //(11) muli  3 65899  3
l_12: reg[3] = reg[3] & 16777215; //(12) bani  3 16777215  3
l_13: reg[5] = 256 > reg[1] ? 1 : 0; //(13) gtir 256  1  5
l_14: if (reg[5] == 0) goto l_15; else goto l_16; //(14) addr  5  2  2
l_15: goto l_17; //(15) addi  2  1  2
l_16: goto l_28; //(16) seti 27  7  2
l_17: reg[5] = 0; //(17) seti  0  3  5
l_18: reg[4] = reg[5] + 1; //(18) addi  5  1  4
l_19: reg[4] = reg[4] * 256; //(19) muli  4 256  4
l_20: reg[4] = reg[4] > reg[1] ? 1 : 0; //(20) gtrr  4  1  4
l_21: if (reg[4] == 0) goto l_22; else goto l_23; //(21) addr  4  2  2
l_22: goto l_24; //(22) addi  2  1  2
l_23: goto l_26; //(23) seti 25  4  2
l_24: reg[5] = reg[5] + 1; //(24) addi  5  1  5
l_25: goto l_18; //(25) seti 17  0  2
l_26: reg[1] = reg[5]; //(26) setr  5  2  1
l_27: goto l_8; //(27) seti  7  4  2
l_28: reg[5] = reg[3] == reg[0] ? 1 : 0; //(28) eqrr  3  0  5
	if (res1 == -1) res1 = reg[3];
	if (ans.end() != ans.find(reg[3]))
		return { res1, last };
	ans.insert(reg[3]);
	last = reg[3];
l_29: if (reg[5] == 0) goto l_30; else goto l_31; //(29) addr  5  2  2
l_30: goto l_6; //(30) seti  5  7  2
l_31:
	return reg;
}