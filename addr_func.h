#ifndef _addr_func_h_ 
#define _addr_func_h_

class AddrFunc {
public:
	virtual int getAddress(int k, int initAddr, int i, int size) const = 0;
};


class DoubleHashing : public AddrFunc {
private:
	int p, q;
public:
	DoubleHashing(int p = 5, int q = 1) : p(p), q(q) {}
	int getAddress(int k, int initAddr, int i, int size) const override {
		return (initAddr + i * (q + (k % p))) % size;
	}
};

#endif
