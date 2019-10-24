static int p;

int funct(int k) {
	int b;
    b = k + k;
	return b;
}

int minus(int k, int m) {
	int n;
	n = k - m;
	return n;
}

void main() {
	int a;
	int b;
	int c;
	c = minus(c,c);
	a = funct(c);
	b = c + funct(a) - 1;
	a = a - 2 + b;
	b = a + b;
	if (a < b || a != c) {
		a = b;
		a = a + b;
	}
	a = a + b;
	if (a > b && !(a - b != c + b)) {
		a = b;
		b = a - b;
		if (a > b) {
		    a = b + a;	
		}
	    else {
		    a = b - a;
		    a = b + a;
		}
	}
	while (a < b) {
		a = b + b;
		b = c;
	}
	a = b;
}