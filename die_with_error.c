#include <stdio.h>
#include <stdlib.h>
#include "die_with_error.h"

void DieWithError(char *errorMessage) {
	perror(errorMessage);
	exit(1);
}
