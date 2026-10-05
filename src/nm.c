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

static t_symbol *extract_32(Elf32_Ehdr *ehdr, size_t file_size, size_t *sym_count)
{

}

static t_symbol *extract_64(Elf64_Ehdr *ehdr, size_t file_size, size_t *sym_count)
{
	
}

static int open_and_map_file(const char *filename, void **mapped_data, size_t *file_size)
{
	int fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		fprintf(stderr, "ft_nm: '%s': No such file\n", filename);
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

static void ft_nm(const char *filename)
{
	void		*mapped_data;
	size_t		file_size;
	t_symbol	*symbols;
	size_t		sym_count;

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
	symbols = NULL;

	if (elf_class == ELFCLASS32)
	{
		if (file_size < sizeof(Elf32_Ehdr))
		{
			fprintf(stderr, "ft_nm: %s: file truncated\n", filename);
			munmap(mapped_data, file_size);
			return;
		}
		Elf32_Ehdr *ehdr = (Elf32_Ehdr *)ptr;
		symbols = extract_32(mapped_data, file_size, &sym_count);
	} else if (elf_class == ELFCLASS64) {
		if (file_size < sizeof(Elf64_Ehdr))
		{
			fprintf(stderr, "ft_nm: %s: file truncated\n", filename);
			munmap(mapped_data, file_size);
			return;
		}
		Elf64_Ehdr *ehdr = (Elf64_Ehdr *)ptr;
		symbols = extract_64(mapped_data, file_size, &sym_count);
	} else {
		fprintf(stderr, "ft_nm: %s: invalid ELF class\n", filename);
		munmap(mapped_data, file_size);
		return;
	}

	if (symbols) {
		//print_symbols(symbols);
		free(symbols);
		printf("Printing symbols here\n");
	}

	if (munmap(mapped_data, file_size) < 0)
		perror("munmap error");
}

int main(int argc, char **argv)
{
	if (argc == 1)
	{
		const char *path = "a.out";
		ft_nm(path);
	}
	else
	{
		for (int i = 1; i < argc; i++)
		{
			printf("%s:\n", argv[i]);
			ft_nm(argv[i]);
		}
	}
	return 0;
}
