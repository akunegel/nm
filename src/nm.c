#include "../inc/nm.h"

// U (undefined): si st_shndx == SHN_UNDEF (le symbole est référencé dans le
// fichier mais déclaré ailleurs) A (absolute): si st_shndx == SHN_ABS
// (l'addresse est fixe) C/c (common): si st_shndx == SHN_COMMON Autrement,
// regarder la section pointée par st_shndx: si code (SHT_PROGBITS avec flag
// exec ou .text): T si global, t si local. si donnée initialisée (.data): D ou
// d si donnée non initialisée (.bss): B ou b
//
// Majusucle quand global (STB_GLOBAL ou STB_WEAK) et minuscule pour local
// (STB_LOCAL)

static int open_and_map_file(const char *filename, void **mapped_data, size_t *file_size)
{
	int fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		perror("error");
		printf("error: %s: No such file or directory\n", filename);
		close(fd);
		return -1;
	}

	struct stat sb;
	if (fstat(fd, &sb) < 0)
	{
		perror("error getting file stats");
		close(fd);
		return -1;
	}

	if (!S_ISREG(sb.st_mode))
	{
		fprintf(stderr, "Error: Not a regular file\n");
		close(fd);
		return -1;
	}

	*file_size = sb.st_size;

	*mapped_data = mmap(NULL, *file_size, PROT_READ, MAP_PRIVATE, fd, 0);
	close(fd);

	if (*mapped_data == MAP_FAILED)
	{
		perror("Error mapping file");
		return -1;
	}

	return 0;
}

static void nm(const char *filename)
{
	void *mapped_data;
	size_t file_size;

	if (open_and_map_file(filename, &mapped_data, &file_size) == -1)
	{
		return;
	}

	unsigned char *ptr = (unsigned char *)mapped_data;
	if (file_size < EI_NIDENT)
	{
		fprintf(stderr, "ft_nm: %s: file format not recognized\n", filename);
		munmap(mapped_data, file_size);
		return;
	}

	if (ptr[0] != 0x7f || ptr[1] != 'E' || ptr[2] != 'L' || ptr[3] != 'F')
	{
		fprintf(stderr, "ft_nm: %s: file format not recognized\n", filename);
		munmap(mapped_data, file_size);
		return;
	}

	unsigned char elf_class = ptr[EI_CLASS];

	if (elf_class == ELFCLASS32)
	{
		if (file_size < sizeof(Elf32_Ehdr))
		{
			fprintf(stderr, "ft_nm: %s: file truncated\n", filename);
			munmap(mapped_data, file_size);
			return;
		}
		Elf32_Ehdr *ehdr = (Elf32_Ehdr *)ptr;
		// handle 32 bit binary
		(void)ehdr; // Just to not trigger werror
		printf("ELFCLASS64 branch took\n");
	} else if (elf_class == ELFCLASS64)
	{
		if (file_size < sizeof(Elf64_Ehdr))
		{
			fprintf(stderr, "ft_nm: %s: file truncated\n", filename);
			munmap(mapped_data, file_size);
			return;
		}
		Elf64_Ehdr *ehdr = (Elf64_Ehdr *)ptr;
		// handle 64 bit binary
		(void)ehdr; // Just to not trigger werror
		printf("ELFCLASS64 branch took\n");
	}

	// ouvrir fichier et recuperer taille
	// map contenu en memoire
	// verifier que le fichier commence bien par les "magic" bytes d'ELF et
	// définir si executable 32-bit ou 64-bit trouver "Section Header Table"
	// grace aux offsets trouver dans le header chercher section SHT_SYMTAB,
	// fallback sur SHT_DYNSYMTAB (à verifier, pas certain que nm fasse ceci par
	// defaut) trouver la "string table" associée (SHT_STRTAB) contenant les
	// noms des symboles extraire et stocker les symboles trouvés, chaque entrée
	// doit contenir: un offset dans la "string table" pour le nom, une valeure
	// (l'addresse), une taille et l'index de section (st_shndx) identifier le
	// symbole correspondant pour l'index de section (voir comm plus haut)
	// afficher par ordre alphabétique
	// unmap et libérer fd
}

int main(int argc, char **argv)
{
	if (argc == 1)
	{
		const char *path = "a.out";
		nm(path);
	}
	else
	{
		for (int i = 1; i < argc; i++)
		{
			printf("%s:\n", argv[i]);
			nm(argv[i]);
		}
	}
	return 0;
}
