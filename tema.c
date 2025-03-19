#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//declararea unei structuri de tip pixel
struct pixel {
		int r;
		int g;
		int b;
	};

//antetul pentru majoritate functiilor utilizate
void read(struct pixel ***image, int *row, int *col);

void write(struct pixel ***image, int row, int col);

void resize(struct pixel ***image, int *row, int *col);

void init_matrix(struct pixel ***image, int row, int col);

void grid(struct pixel ***image, int row, int col, int ***grid_puncte, int a);

void outline(int k, int start_i, int start_j, struct pixel ***image);

void march(struct pixel ***image, int row, int col, int **grid_puncte);

void free_matrix_pixel(int row, struct pixel ***image);

void free_matrix_int(int row, int ***matrix);

void init_outline(void);

int main(void)
{
	struct pixel **image = NULL;
	int row, col, **grid_puncte = NULL;
	char action[10];
	//citirea actiunilor ce urmeaza sa aiba loc
	scanf("%s", action);
	while (strcmp(action, "EXIT")) {
		if (!strcmp(action, "READ")) {
			free_matrix_int(row / 4 + 1, &grid_puncte);
			read(&image, &row, &col);
		} else if (!strcmp(action, "WRITE")) {
			write(&image, row, col);
		} else if (!strcmp(action, "RESIZE")) {
			resize(&image, &row, &col);
		} else if (!strcmp(action, "GRID")) {
			grid(&image, row, col, &grid_puncte, 1);
		} else if (!strcmp(action, "MARCH")) {
			march(&image, row, col, grid_puncte);
		} else if (!strcmp(action, "INIT_outline")) {
			init_outline();
		}
		scanf("%s", action);
	}
	//eliberari de memorie
	free_matrix_int(row / 4 + 1, &grid_puncte);
	free_matrix_pixel(row, &image);
	printf("Gigel a terminat");

}

//citirea elementelor necesare determinarii imaginii
void read(struct pixel ***image, int *row, int *col)
{
	int i, j, max_pixel;
	free_matrix_pixel(*row, &(*image));
	char type[3] = "P6";
	scanf("%s", type);
	if (strcmp(type, "P3") == 0) {
		scanf("%d%d", &(*row), &(*col));
		scanf("%d", &max_pixel);
		*image = (struct pixel **)malloc(*row * sizeof(struct pixel *));
		for (i = 0; i < *row; i++) {
			(*image)[i] = (struct pixel *)malloc(*col * sizeof(struct pixel));

		}
		for (i = 0; i < *row; i++) {
			for (j = 0; j < *col; j++) {
				int r, g, b, eroare;
				//cazurile de eroare la citire
				eroare = scanf("%d%d%d", &r, &g, &b);
				if (eroare != 3) {
					printf("Eroare: eroare citire pixeli");
					free_matrix_pixel(*row, image);
					exit(0);
				}
				if (r > 255 || g > 255 || b > 255 || r < 0 || g < 0 || b < 0) {
					printf("Eroare: valoare pixel necorespunzatoare");
					free_matrix_pixel(*row, image);
					*image = NULL;
					exit(0);
				}
				(*image)[i][j].r = r;
				(*image)[i][j].g = g;
				(*image)[i][j].b = b;

			}

		}
	} else {
		//eroare de citire
		printf("Eroare: trebuie sa fie P3\n");
		exit(0);
	}
	//imaginea a fost citita cu succes
	printf("Imagine citita cu succes [%d-%d]\n", *row, *col);
}

//functia de afisare
void write(struct pixel ***image, int row, int col)
{
	int i, j;
	printf("P3\n%d %d\n255\n", row, col);
	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			printf("%d %d", (*image)[i][j].r, (*image)[i][j].g);
			printf(" %d ", (*image)[i][j].b);
			printf("\n");
		}
	}
}

//functie oentru alocarea dinamica a unei matrice de tip int
void allocation_matrix_int(int ***matrix, int row, int col)
{
	int i;
	*matrix = (int **)calloc(row, sizeof(int *));
	if (!*matrix) {
		printf("Eroare");
	}
	for (i = 0; i < row; i++) {
		(*matrix)[i] = (int *)calloc(col, sizeof(int));
		if (!(*matrix)[i]) {
			printf("Eroare la alocare %d", i);
		}

	}
}

//compararea cu valoarea sigma pentru determinarea grid-ului
int sigma(int x)
{
	if (x > 200) {
		return 0;
	} else {
		return 1;
	}
}

//functia de redimensionare a imaginii
void resize(struct pixel ***image, int *row, int *col)
{
	struct pixel **aux = NULL;
	int i, j;

	aux = (struct pixel **)malloc(*row * sizeof(struct pixel *));
	for (i = 0; i < *row; i++) {
		aux[i] = (struct pixel *)malloc(*col * sizeof(struct pixel));

	}
	for (i = 0; i < *row; i++) {
		for (j = 0; j < *col; j++) {
			aux[i][j] = (*image)[i][j];

		}
	}
	//eliberari de memorie
	free_matrix_pixel(*row, &(*image));

	*row = 4 * (*row);
	*col = 4 * (*col);

	*image = (struct pixel **)malloc(*row * sizeof(struct pixel *));

	for (i = 0; i < *row; i++) {
		(*image)[i] = (struct pixel *)malloc(*col * sizeof(struct pixel));

	}

	for (i = 0; i < (*row) / 4; i++) {
		for (j = 0; j < (*col) / 4; j++) {
			(*image)[i * 4][j * 4] = aux[i][j];

		}

	}
	//eliberari de memorie
	free_matrix_pixel(*row / 4, &aux);
	for (i = 0; i < *row; i++) {
		for (j = 0; j < *col; j++) {
			if (i % 4 != 0) {
				(*image)[i][j] = (*image)[i - 1][j];

			} else {
				if (j % 4 != 0) {
					(*image)[i][j] = (*image)[i][j - 1];

				}

			}

		}

	}
	printf("Imagine redimensionata cu succes [%d-%d]\n", *row, *col);
}

void grid(struct pixel ***image, int row, int col, int ***grid_puncte, int a)
{
	int n = row, m = col, i, j, **grid_board;
	allocation_matrix_int(&grid_board, n + 2, m + 2);
	allocation_matrix_int(&(*grid_puncte), n / 4 + 1, m / 4 + 1);
	//crearea gridului bordat
	for (i = 0; i < n + 2; i++) {
		for (j = 0; j < m + 2; j++) {
			grid_board[i][j] = -1;
		}
	}
	for (i = 0; i < n; i++) {
		for (j = 0; j < m; j++) {
			grid_board[i + 1][j + 1] =
			((*image)[i][j].r + (*image)[i][j].g +
			(*image)[i][j].b) / 3;
		}
	}
	//crearea gridului de puncte
	for (i = 1; i < n + 1; i = i + 4) {
		for (j = 1; j < m + 1; j = j + 4) {
			int k = 4, x = 0;
			if (grid_board[i + 1][j + 1] == -1) {
				k--;
			} else {
				x = x + grid_board[i + 1][j + 1];
			}
			if (grid_board[i + 1][j - 1] == -1) {
				k--;
			} else {
				x = x + grid_board[i + 1][j - 1];
			}
			if (grid_board[i - 1][j + 1] == -1) {
				k--;
			} else {
				x = x + grid_board[i - 1][j + 1];
			}
			if (grid_board[i - 1][j - 1] == -1) {
				k--;
			} else {
				x = x + grid_board[i - 1][j - 1];
			}
			(*grid_puncte)[(i - 1) / 4][(j - 1) / 4] =
			(x + grid_board[i][j]) / (k + 1);
			(*grid_puncte)[(i - 1) / 4][(j - 1) / 4] =
			sigma((*grid_puncte)[(i - 1) / 4][(j - 1) / 4]);
		}
	}
	//completarea gridului de puncte
	for (j = 0; j < m / 4 + 1; j++) {
		if (j == 0) {
			(*grid_puncte)[n / 4][j] = grid_board[n][j + 2];
		} else if (j == m / 4) {
			(*grid_puncte)[n / 4][j] = grid_board[n][4 * j];
		} else {
			(*grid_puncte)[n / 4][j] = (grid_board[n][4 * j + 2] +
			grid_board[n][4 * j - 2]) / 2;
		}
		(*grid_puncte)[n / 4][j] = sigma((*grid_puncte)[n / 4][j]);
	}
	for (i = 0; i < n / 4; i++) {
		if (i == 0) {
			(*grid_puncte)[i][m / 4] = grid_board[i + 2][m];
		} else if (i == n / 4 - 1) {
			(*grid_puncte)[i][m / 4] = grid_board[4 * i][m];
		} else {
			(*grid_puncte)[i][m / 4] = (grid_board[4 * i + 2][m] +
			grid_board[4 * i - 2][m]) / 2;
		}
		(*grid_puncte)[i][m / 4] = sigma((*grid_puncte)[i][m / 4]);
	}
	if (a == 1) {
		printf("Grid calculat cu succes [%d-%d]\n", row / 4, col / 4);
		for (i = 0; i < n / 4; i++) {
			for (j = 0; j < m / 4; j++) {
				printf("%d ", (*grid_puncte)[i][j]);
			}
		printf("\n");
		}
	}
	free_matrix_int(n + 2, &grid_board);
}

//functii pentru afisarea conturului
void outline0(void)
{
	int i, j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			printf("0 ");
		}
		printf("\n");
	}
	printf("\n");
}

void outline1(void)
{
	int i, j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			if (i == 2 && j == 0) {
				printf("255 ");

			} else if (i == 3 && j == 0) {
				printf("180 ");

			} else if (i == 3 && j == 1) {
				printf("255 ");

			} else {
				printf("0 ");

			}
		}
		printf("\n");
	}
	printf("\n");
}

void outline2(void)
{
	int i, j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			if (i == 2 && j == 3) {
				printf("255 ");

			} else if (i == 3 && j == 3) {
				printf("180 ");

			} else if (i == 3 && j == 2) {
				printf("255 ");

			} else {
				printf("0 ");

			}
		}
		printf("\n");
	}
	printf("\n");
}

void outline3(void)
{
	int i, j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			if (i < 2) {
				printf("0 ");

			} else if (i == 2) {
				printf("255 ");

			} else {
				printf("180 ");

			}

		}
		printf("\n");
	}
	printf("\n");
}

void outline4(void)
{
	int i, j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			if (i == 1 && j == 3) {
				printf("255 ");

			} else if (i == 0 && j == 3) {
				printf("180 ");

			} else if (i == 0 && j == 2) {
				printf("255 ");

			} else {
				printf("0 ");

			}
		}
		printf("\n");
	}
	printf("\n");
}

void outline5(void)
{
	int i, j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			if (i == 1 && j == 3) {
				printf("255 ");

			} else if (i == 0 && j == 3) {
				printf("180 ");

			} else if (i == 0 && j == 2) {
				printf("255 ");

			} else if (i == 2 && j == 0) {
				printf("255 ");

			} else if (i == 3 && j == 0) {
				printf("180 ");

			} else if (i == 3 && j == 1) {
				printf("255 ");

			} else {
				printf("0 ");

			}
		}
		printf("\n");
	}
	printf("\n");
}

void outline6(void)
{
	int i, j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			if (j < 2) {
				printf("0 ");

			} else if (j == 2) {
				printf("255 ");

			} else {
				printf("180 ");

			}

		}
		printf("\n");
	}
	printf("\n");
}

void outline7(void)
{
	int i, j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			if (i == 0 && j == 1) {
				printf("255 ");

			} else if (i == 0 && j == 0) {
				printf("0 ");

			} else if (i == 1 && j == 0) {
				printf("255 ");

			} else {
				printf("180 ");

			}
		}
		printf("\n");
	}
	printf("\n");
}

void outline8(void)
{
	int i, j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			if (i == 0 && j == 1) {
				printf("255 ");

			} else if (i == 0 && j == 0) {
				printf("180 ");

			} else if (i == 1 && j == 0) {
				printf("255 ");

			} else {
				printf("0 ");

			}
		}
		printf("\n");
	}
	printf("\n");
}

void outline9(void)
{
	int i, j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			if (j < 2) {
				printf("180 ");

			} else if (j == 2) {
				printf("255 ");

			} else {
				printf("0 ");

			}

		}
		printf("\n");
	}
	printf("\n");
}

void outline10(void)
{
	int i, j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			if (i == 0 && j == 1) {
				printf("255 ");

			} else if (i == 0 && j == 0) {
				printf("180 ");

			} else if (i == 1 && j == 0) {
				printf("255 ");

			} else if (i == 2 && j == 3) {
				printf("255 ");

			} else if (i == 3 && j == 3) {
				printf("180 ");

			} else if (i == 3 && j == 2) {
				printf("255 ");

			} else {
				printf("0 ");

			}
		}
		printf("\n");
	}
	printf("\n");
}

void outline11(void)
{
	int i, j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			if (i == 1 && j == 3) {
				printf("255 ");

			} else if (i == 0 && j == 3) {
				printf("0 ");

			} else if (i == 0 && j == 2) {
				printf("255 ");

			} else {
				printf("180 ");

			}
		}
		printf("\n");
	}
	printf("\n");
}

void outline12(void)
{
	int i, j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			if (i < 2) {
				printf("180 ");

			} else if (i == 2) {
				printf("255 ");

			} else {
				printf("0 ");

			}

		}
		printf("\n");
	}
	printf("\n");
}

void outline13(void)
{
	int i, j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			if (i == 2 && j == 3) {
				printf("255 ");

			} else if (i == 3 && j == 3) {
				printf("0 ");

			} else if (i == 3 && j == 2) {
				printf("255 ");

			} else {
				printf("180 ");

			}
		}
		printf("\n");
	}
	printf("\n");
}

void outline14(void)
{
	int i, j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			if (i == 2 && j == 0) {
				printf("255 ");

			} else if (i == 3 && j == 0) {
				printf("0 ");

			} else if (i == 3 && j == 1) {
				printf("255 ");

			} else {
				printf("180 ");

			}
		}
		printf("\n");
	}
	printf("\n");
}

void outline15(void)
{
	int i, j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			printf("255 ");

		}
		printf("\n");

	}
	printf("\n");

}

// functie pentru atribuire valori
void assign(struct pixel ***image, int value, int i, int j)
{
	(*image)[i][j].r = value;
	(*image)[i][j].g = value;
	(*image)[i][j].b = value;
}

//afisare contur
void init_outline(void)
{
	printf("P3\n4 4\n255\n");
	outline0();
	outline1();
	outline2();
	outline3();
	outline4();
	outline5();
	outline6();
	outline7();
	outline8();
	outline9();
	outline10();
	outline11();
	outline12();
	outline13();
	outline14();
	outline15();

}

//creare matrice cu contururi
void outline_matrix_0(int start_i, int start_j, struct pixel ***image)
{
	int i, j;
	for (i = start_i; i < start_i + 4; i++) {
		for (j = start_j; j < start_j + 4; j++) {
			assign(&(*image), 0, i, j);
		}
	}
}

void outline_matrix_1(int start_i, int start_j, struct pixel ***image)
{
	int i, j;
	for (i = start_i; i < start_i + 4; i++) {
		for (j = start_j; j < start_j + 4; j++) {
			if (i == start_i + 2 && j == start_j) {
				assign(&(*image), 255, i, j);
			} else if (i == start_i + 3 && j == start_j) {
				assign(&(*image), 180, i, j);
			} else if (i == start_i + 3 && j == start_j + 1) {
				assign(&(*image), 255, i, j);
			} else {
				assign(&(*image), 0, i, j);
			}
		}
	}
}

void outline_matrix_2(int start_i, int start_j, struct pixel ***image)
{
	int i, j;
	for (i = start_i; i < start_i + 4; i++) {
		for (j = start_j; j < start_j + 4; j++) {
			if (i == start_i + 2 && j == start_j + 3) {
				assign(&(*image), 255, i, j);
			} else if (i == start_i + 3 && j == start_j + 3) {
				assign(&(*image), 180, i, j);
			} else if (i == start_i + 3 && j == start_j + 2) {
				assign(&(*image), 255, i, j);
			} else {
				assign(&(*image), 0, i, j);
			}
		}
	}
}

void outline_matrix_3(int start_i, int start_j, struct pixel ***image)
{
	int i, j;
	for (i = start_i; i < start_i + 4; i++) {
		for (j = start_j; j < start_j + 4; j++) {
			if (i < start_i + 2) {
				assign(&(*image), 0, i, j);
			} else if (i == start_i + 2) {
				assign(&(*image), 255, i, j);
			} else {
				assign(&(*image), 180, i, j);
			}
		}
	}
}

void outline_matrix_4(int start_i, int start_j, struct pixel ***image)
{
	int i, j;
	for (i = start_i; i < start_i + 4; i++) {
		for (j = start_j; j < start_j + 4; j++) {
			if (i == start_i + 1 && j == start_j + 3) {
				assign(&(*image), 255, i, j);
			} else if (i == start_i && j == start_j + 3) {
				assign(&(*image), 180, i, j);
			} else if (i == start_i && j == start_j + 2) {
				assign(&(*image), 255, i, j);
			} else {
				assign(&(*image), 0, i, j);
			}
		}
	}
}

void outline_matrix_5(int start_i, int start_j, struct pixel ***image)
{
	int i, j;
	for (i = start_i; i < start_i + 4; i++) {
		for (j = start_j; j < start_j + 4; j++) {
			if (i == start_i + 1 && j == start_j + 3) {
				assign(&(*image), 255, i, j);
			} else if (i == start_i && j == start_j + 3) {
				assign(&(*image), 180, i, j);
			} else if (i == start_i && j == start_j + 2) {
				assign(&(*image), 255, i, j);
			} else if (i == start_i + 2 && j == start_j) {
				assign(&(*image), 255, i, j);
			} else if (i == start_i + 3 && j == start_j) {
				assign(&(*image), 180, i, j);
			} else if (i == start_i + 3 && j == start_j + 1) {
				assign(&(*image), 255, i, j);
			} else {
				assign(&(*image), 0, i, j);
			}
		}
	}
}

void outline_matrix_6(int start_i, int start_j, struct pixel ***image)
{
	int i, j;
	for (i = start_i; i < start_i + 4; i++) {
		for (j = start_j; j < start_j + 4; j++) {
			if (j < start_j + 2) {
				assign(&(*image), 0, i, j);
			} else if (j == start_j + 2) {
				assign(&(*image), 255, i, j);
			} else {
				assign(&(*image), 180, i, j);
			}
		}
	}
}

void outline_matrix_7(int start_i, int start_j, struct pixel ***image)
{
	int i, j;
	for (i = start_i; i < start_i + 4; i++) {
		for (j = start_j; j < start_j + 4; j++) {
			if (i == start_i && j == start_j + 1) {
				assign(&(*image), 255, i, j);
			} else if (i == start_i && j == start_j) {
				assign(&(*image), 0, i, j);
			} else if (i == start_i + 1 && j == start_j) {
				assign(&(*image), 255, i, j);
			} else {
				assign(&(*image), 180, i, j);
			}
		}
	}
}

void outline_matrix_8(int start_i, int start_j, struct pixel ***image)
{
	int i, j;
	for (i = start_i; i < start_i + 4; i++) {
		for (j = start_j; j < start_j + 4; j++) {
			if (i == start_i && j == start_j + 1) {
				assign(&(*image), 255, i, j);
			} else if (i == start_i && j == start_j) {
				assign(&(*image), 180, i, j);
			} else if (i == start_i + 1 && j == start_j) {
				assign(&(*image), 255, i, j);
			} else {
				assign(&(*image), 0, i, j);
			}
		}
	}
}

void outline_matrix_9(int start_i, int start_j, struct pixel ***image)
{
	int i, j;
	for (i = start_i; i < start_i + 4; i++) {
		for (j = start_j; j < start_j + 4; j++) {
			if (j < start_j + 1) {
				assign(&(*image), 180, i, j);
			} else if (j == start_j + 1) {
				assign(&(*image), 255, i, j);
			} else {
				assign(&(*image), 0, i, j);
			}
		}
	}
}

void outline_matrix_10(int start_i, int start_j, struct pixel ***image)
{
	int i, j;
	for (i = start_i; i < start_i + 4; i++) {
		for (j = start_j; j < start_j + 4; j++) {
			if (i == start_i && j == start_j + 1) {
				assign(&(*image), 255, i, j);
			} else if (i == start_i && j == start_j) {
				assign(&(*image), 180, i, j);
			} else if (i == start_i + 1 && j == start_j) {
				assign(&(*image), 255, i, j);
			} else if (i == start_i + 2 && j == start_j + 3) {
				assign(&(*image), 255, i, j);
			} else if (i == start_i + 3 && j == start_j + 3) {
				assign(&(*image), 180, i, j);
			} else if (i == start_i + 3 && j == start_j + 2) {
				assign(&(*image), 255, i, j);
			} else {
				assign(&(*image), 0, i, j);
			}
		}
	}
}

void outline_matrix_11(int start_i, int start_j, struct pixel ***image)
{
	int i, j;
	for (i = start_i; i < start_i + 4; i++) {
		for (j = start_j; j < start_j + 4; j++) {
			if (i == start_i + 1 && j == start_j + 3) {
				assign(&(*image), 255, i, j);
			} else if (i == start_i && j == start_j + 3) {
				assign(&(*image), 0, i, j);
			} else if (i == start_i && j == start_j + 2) {
				assign(&(*image), 255, i, j);
			} else {
				assign(&(*image), 180, i, j);
			}
		}
	}
}

void outline_matrix_12(int start_i, int start_j, struct pixel ***image)
{
	int i, j;
	for (i = start_i; i < start_i + 4; i++) {
		for (j = start_j; j < start_j + 4; j++) {
			if (i < start_i + 1) {
				assign(&(*image), 180, i, j);
			} else if (i == start_i + 1) {
				assign(&(*image), 255, i, j);
			} else {
				assign(&(*image), 0, i, j);
			}
		}
	}
}

void outline_matrix_13(int start_i, int start_j, struct pixel ***image)
{
	int i, j;
	for (i = start_i; i < start_i + 4; i++) {
		for (j = start_j; j < start_j + 4; j++) {
			if (i == start_i + 2 && j == start_j + 3) {
				assign(&(*image), 255, i, j);
			} else if (i == start_i + 3 && j == start_j + 3) {
				assign(&(*image), 0, i, j);
			} else if (i == start_i + 3 && j == start_j + 2) {
				assign(&(*image), 255, i, j);
			} else {
				assign(&(*image), 180, i, j);
			}
		}
	}
}

void outline_matrix_14(int start_i, int start_j, struct pixel ***image)
{
	int i, j;
	for (i = start_i; i < start_i + 4; i++) {
		for (j = start_j; j < start_j + 4; j++) {
			if (i == start_i + 2 && j == start_j) {
				assign(&(*image), 255, i, j);
			} else if (i == start_i + 3 && j == start_j) {
				assign(&(*image), 0, i, j);
			} else if (i == start_i + 3 && j == start_j + 1) {
				assign(&(*image), 255, i, j);
			} else {
				assign(&(*image), 180, i, j);
			}
		}
	}
}

void outline_matrix_15(int start_i, int start_j, struct pixel ***image)
{
	int i, j;
	for (i = start_i; i < start_i + 4; i++) {
		for (j = start_j; j < start_j + 4; j++) {
			(*image)[i][j].r = 255;
			(*image)[i][j].g = 255;
			(*image)[i][j].b = 255;
		}
	}
}

void outline(int k, int start_i, int start_j, struct pixel ***image)
{
	if (k == 0) {
		outline_matrix_0(start_i, start_j, &(*image));
	} else if (k == 1) {
		outline_matrix_1(start_i, start_j, &(*image));
	} else if (k == 2) {
		outline_matrix_2(start_i, start_j, &(*image));
	} else if (k == 3) {
		outline_matrix_3(start_i, start_j, &(*image));
	} else if (k == 4) {
		outline_matrix_4(start_i, start_j, &(*image));
	} else if (k == 5) {
		outline_matrix_5(start_i, start_j, &(*image));
	} else if (k == 6) {
		outline_matrix_6(start_i, start_j, &(*image));
	} else if (k == 7) {
		outline_matrix_7(start_i, start_j, &(*image));
	} else  if (k == 8) {
		outline_matrix_8(start_i, start_j, &(*image));
	} else if (k == 9) {
		outline_matrix_9(start_i, start_j, &(*image));
	} else if (k == 10) {
		outline_matrix_10(start_i, start_j, &(*image));
	} else if (k == 11) {
		outline_matrix_11(start_i, start_j, &(*image));
	} else if (k == 12) {
		outline_matrix_12(start_i, start_j, &(*image));
	} else if (k == 13) {
		outline_matrix_13(start_i, start_j, &(*image));
	} else if (k == 14) {
		outline_matrix_14(start_i, start_j, &(*image));
	} else if (k == 15) {
		outline_matrix_15(start_i, start_j, &(*image));
	}
}

// functia de march
void march(struct pixel ***image, int row, int col, int **grid_puncte)
{
	int i, j;
	//creare grid
	grid(&(*image), row, col, &grid_puncte, 0);
	for (i = 0; i < row / 4; i++) {
		for (j = 0; j < col / 4; j++) {
			outline((grid_puncte[i][j] * 8 +
			grid_puncte[i][j + 1] * 4 +
			grid_puncte[i + 1][j + 1] * 2 +
			grid_puncte[i + 1][j] * 1), 4 * i, 4 * j, &(*image));
		}
	}
	//afisare
	printf("Marching Squares aplicat cu succes [%d-%d]\n", row, col);
	//eliberare
	free_matrix_int(row / 4 + 1, &grid_puncte);
}

//functii pentru eliberari
void free_matrix_pixel(int row, struct pixel ***image)
{
	if (!*image) {
		return;
	}
	for (int i = 0; i < row; i++) {
		free((*image)[i]);

	}
	free(*image);
	*image = NULL;
}

void free_matrix_int(int row, int ***matrix)
{
	if (!*matrix) {
		return;
	}
	for (int i = 0; i < row; i++) {
		free((*matrix)[i]);

	}
	free(*matrix);
	*matrix = NULL;
}
