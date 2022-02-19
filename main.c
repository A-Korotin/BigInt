#include "stdio.h"
#include "stdlib.h"
#include "string.h"

typedef struct BigInteger{
	int size;
	int* data;
}BigInteger;


BigInteger* newInteger() {
	BigInteger* i =(BigInteger*) malloc(sizeof(BigInteger));
	return i;
}

void destroy(BigInteger* integer) {
	free(integer->data);
}

void strip(BigInteger*& integer) {
	int leadingZeros = 0;
	while (integer->data[leadingZeros] == 0)
		leadingZeros++;
	BigInteger* newInt = newInteger();
	newInt->size = integer->size - leadingZeros;
	newInt->data = (int*)malloc(newInt->size * sizeof(int));
	memcpy(newInt->data, integer->data + leadingZeros, newInt->size*sizeof(int));
	BigInteger* old = integer;
	integer = newInt;
	destroy(old);
}

BigInteger* integerFromString(const char* input) {
	BigInteger* integer = newInteger();
	unsigned int leadingZerosSize = 0;
	while (input[leadingZerosSize] == '0') { leadingZerosSize++; }
	integer->size = strlen(input) - leadingZerosSize;
	if (integer->size == 0) {
		integer->size++;
		integer->data = (int*)malloc(integer->size * sizeof(int));
		integer->data[0] = 0;
	}
	else {
		integer->data = (int*)malloc(integer->size * sizeof(int));
		for (int i = 0; i < integer->size; ++i)
			integer->data[i] = input[leadingZerosSize + i] - '0';
	}
	return integer;
}

BigInteger* integerFromInt(int input) {
	int copy = input * 10;
	int nDigits = 0;
	while ((copy /= 10) != 0) { nDigits++; }
	BigInteger* out = newInteger();
	out->data = (int*)malloc(sizeof(int) * nDigits);
	out->size = nDigits;
	for (int i = 0; i < out->size; ++i) {
		out->data[out->size-i-1] = input % 10;
		input /= 10;
	}
	return out;
}

char* integerToString(BigInteger* integer) {
	char* out = (char*)malloc((integer->size + 1) * sizeof(char));
	for (int i = 0; i < integer->size; ++i)
		out[i] = integer->data[i] + '0';
	out[integer->size] = '\0';
	return out;
}


BigInteger* add(BigInteger* a, BigInteger* b) {
	BigInteger* c = newInteger();
	c->size = (a->size > b->size ? a->size : b->size) + 1;
	c->data = (int*)malloc(c->size + sizeof(int));
	memset(c->data, 0, c->size*sizeof(int));
	int i = a->size - 1, j = b->size - 1;
	int k = c->size - 1;
	int carry = 0;
	int tmp;
	while (i >= 0 || j >= 0 || carry > 0) {
		if (i >= 0 && j >= 0)
			tmp = a->data[i] + b->data[j];
		else if (i >= 0)
			tmp = a->data[i];
		else if (j >= 0) 
			tmp = b->data[j];
		else
			tmp = 0;

		tmp += carry;
		carry = tmp / 10;
		c->data[k] = tmp % 10;
		i--;
		j--;
		k--;
	}
	strip(c);
	return c;
}

BigInteger* multiply(BigInteger* a, BigInteger* b) {
	BigInteger* c = newInteger();
	c->size = a->size + b->size;
	c->data = (int*) malloc(c->size * sizeof(int));
	memset(c->data, 0, c->size*sizeof(int));

	int i = a->size - 1;
	int j = b->size - 1;
	int k = c->size - 1;

	int carry = 0;
	int tmp;
	int pushLeft = 0;
	while (i >= 0) {
		k = c->size - 1 - pushLeft++;
		j = b->size - 1;
		while (j >= 0 || carry > 0) {
			if (j >= 0)
				tmp = a->data[i] * b->data[j];
			else
				tmp = 0;
			tmp += carry;
			carry = tmp / 10;
			c->data[k] += tmp % 10;
			carry += c->data[k] / 10;
			c->data[k] = c->data[k] % 10;
			j--;
			k--;
		}
		i--;
	}
	strip(c);
	return c;
}

void print(BigInteger* integer) {
	char* data = integerToString(integer);
	puts(data);
	free(data);
}


BigInteger* factorial(int n) {
	BigInteger* out = integerFromString("1");
	for (int i = 1; i <= n; ++i) {
		BigInteger* tmp = out;
		BigInteger* number = integerFromInt(i);
		out = multiply(tmp, number);
		destroy(tmp);
		destroy(number);
	}
	return out;
}

int main() {
	BigInteger* fac = factorial(200);
	print(fac);
	printf("%d", fac->size);

	return 0;
}