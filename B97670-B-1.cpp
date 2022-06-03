int strcmp(char *t1, char *t2) {
	int result = 0;
	int iterator = 0;
	while( t1[iterator]!="\0" && t2[iterator]!="\0" ) {
		if (t1[iterator] < t2[iterator]) {
			resultado = -1;
		}
		if (t1[iterator] > t2[iterator]) {
			resultado = 1;
		}
	}

	return resultado;
}