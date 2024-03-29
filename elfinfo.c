
/************************************************************************************
 * @file    : elfinfo.c 
 * @brief   : elf information
 *           
 *           
 * @version : 2.0
 * @date    : 2014/06/30
 * @author  : tp.joo@daum.net
 *
 ************************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>  
#include <assert.h>	/* assert() */
#include <unistd.h>
#include <memory.h> 

#include "common.h"
#include "feature.h"
#include "hash.h"




extern int verbose;
extern int g_iUpper;




#if ELF2BIN

#define ELF_LITTLE_ENDIAN 			1

#define	ELFCLASSNONE		0	/* Invalid class */
#define	ELFCLASS32	      	1   /* 32 bit objects */
#define	ELFCLASS64	       	2   /* 64 bit objects */
#define	ELFCLASSNUM			3   /* ELF class number */


#define ET_NONE  	0    /* 파일의 타입이 없음 */
#define ET_REL    	1    /* 재배치 가능 파일 */
#define ET_EXEC   	2    /* 실행 화일 */
#define ET_DYN    	3    /* 공유 object 파일 */
#define ET_CORE   	4    /* core 파일 */
#define ET_LOPROC 	0xff00 /* 프로세서에 의존적인 파일 */


#define EM_NONE  		0    /* 특정 machine을 구분하지 않음 */
#define EM_M32   		1     /* AT&T WE32100 */
#define EM_SPARC 		2     /* SPARC */
#define EM_386   		3      /* Intel 80386 */
#define EM_68K   		4      /* Motorola 68000 */
#define EM_88K   		5      /* Motorola 88000 */
#define EM_486   		6      /* 사용되지 않음 */ 
#define EM_860   		7      /* Intel 80860 */

#define EM_MIPS_RS4_BE 10	/* MIPS R4000 big-endian */
#define EM_PARISC      15	/* HPPA */
#define EM_SPARC32PLUS 18/* Sun's "v8plus" */
#define EM_PPC	   		20	/* PowerPC */
#define EM_SH	   		42	/* SuperH */
#define EM_SPARCV9   	43	/* SPARC v9 64-bit */
#define EM_IA_64	   50	/* HP/Intel IA-64 */
#define EM_X8664       62  /* AMD x86-64 */
#define EM_ALPHA	   0x9026 /* Alpha */
#define EM_S390        0xA390 /* IBM S390 */


/* 
 * These typedefs need to be handled better - 
 *  doesn't work on 64-bit machines.  Note: 
 *  there currently isn't a 64-bit ABI. 
 */ 
typedef u_int32_t   Elf32_Addr; /* Unsigned program address */ 
typedef u_int32_t   Elf32_Off;  /* Unsigned file offset */ 
typedef int32_t     Elf32_Sword;    /* Signed large integer */ 
typedef u_int32_t   Elf32_Word; /* Unsigned large integer */ 
typedef u_int16_t   Elf32_Half; /* Unsigned medium integer */ 
 
/* e_ident[] identification indexes */ 
#define EI_MAG0     0       /* file ID */ 
#define EI_MAG1     1       /* file ID */ 
#define EI_MAG2     2       /* file ID */ 
#define EI_MAG3     3       /* file ID */ 
#define EI_CLASS    4       /* file class */ 
#define EI_DATA     5       /* data encoding */ 
#define EI_VERSION  6       /* ELF header version */ 
#define EI_PAD      7       /* start of pad bytes */ 
#define EI_NIDENT   16      /* Size of e_ident[] */ 
 
/* e_ident[] magic number */ 
#define ELFMAG0     0x7f        /* e_ident[EI_MAG0] */ 
#define ELFMAG1     'E'     /* e_ident[EI_MAG1] */ 
#define ELFMAG2     'L'     /* e_ident[EI_MAG2] */ 
#define ELFMAG3     'F'     /* e_ident[EI_MAG3] */ 
#define ELFMAG      "\177ELF"   /* magic */ 
#define SELFMAG     4       /* size of magic */ 
 
/* e_ident[] file class */ 
#define ELFCLASSNONE    0       /* invalid */ 
#define ELFCLASS32  1       /* 32-bit objs */ 
#define ELFCLASS64  2       /* 64-bit objs */ 
#define ELFCLASSNUM 3       /* number of classes */ 
 
/* e_ident[] data encoding */ 
#define ELFDATANONE 0       /* invalid */ 
#define ELFDATA2LSB 1       /* Little-Endian */ 
#define ELFDATA2MSB 2       /* Big-Endian */ 
#define ELFDATANUM  3       /* number of data encode defines */ 
 
/* e_ident */ 
#define IS_ELF(ehdr) ((ehdr).e_ident[EI_MAG0] == ELFMAG0 && \
                      (ehdr).e_ident[EI_MAG1] == ELFMAG1 && \
                      (ehdr).e_ident[EI_MAG2] == ELFMAG2 && \
                      (ehdr).e_ident[EI_MAG3] == ELFMAG3)


/* ELF Header */ 
typedef struct elfhdr 
{ 
    unsigned char e_ident[EI_NIDENT];   /* ELF Identification */ 
    Elf32_Half e_type;      /* object file type */ 
    Elf32_Half e_machine;   /* machine */ 
    Elf32_Word e_version;   /* object file version */ 
    Elf32_Addr e_entry;     /* virtual entry point */ 
    Elf32_Off e_phoff;      /* program header table offset */ 
    Elf32_Off e_shoff;      /* section header table offset */ 
    Elf32_Word e_flags;     /* processor-specific flags */ 
    Elf32_Half e_ehsize;    /* ELF header size */ 
    Elf32_Half e_phentsize; /* program header entry size */ 
    Elf32_Half e_phnum;     /* number of program header entries */ 
    Elf32_Half e_shentsize; /* section header entry size */ 
    Elf32_Half e_shnum;     /* number of section header entries */ 
    Elf32_Half e_shstrndx;  /* section header table's "section header 
                   string table" entry offset */ 
} Elf32_Ehdr; 
 
/* e_type */ 
#define ET_NONE     0       /* No file type */ 
#define ET_REL      1       /* relocatable file */ 
#define ET_EXEC     2       /* executable file */ 
#define ET_DYN      3       /* shared object file */ 
#define ET_CORE     4       /* core file */ 
#define ET_NUM      5       /* number of types */ 
#define ET_LOPROC   0xff00      /* reserved range for processor */ 
#define ET_HIPROC   0xffff      /*  specific e_type */ 
 
/* e_machine */ 
#define EM_NONE     0       /* No Machine */ 
#define EM_M32      1       /* AT&T WE 32100 */ 
#define EM_SPARC    2       /* SPARC */ 
#define EM_386      3       /* Intel 80386 */ 
#define EM_68K      4       /* Motorola 68000 */ 
#define EM_88K      5       /* Motorola 88000 */ 
#define EM_486      6       /* Intel 80486 - unused? */ 
#define EM_860      7       /* Intel 80860 */ 
#define EM_MIPS     8       /* MIPS R3000 Big-Endian only */ 
/*  
 * Don't know if EM_MIPS_RS4_BE, 
 * EM_SPARC64, EM_PARISC, 
 * or EM_PPC are ABI compliant 
 */ 
#define EM_MIPS_RS4_BE  10      /* MIPS R4000 Big-Endian */ 
#define EM_SPARC64  11      /* SPARC v9 64-bit unoffical */ 
#define EM_PARISC   15      /* HPPA */ 
#define EM_PPC      20      /* PowerPC */ 
#define EM_NUM      13      /* number of machine types */ 
 
/* Version */ 
#define EV_NONE     0       /* Invalid */ 
#define EV_CURRENT  1       /* Current */ 
#define EV_NUM      2       /* number of versions */ 
 
/* Section Header */ 
typedef struct 
{ 
    Elf32_Word sh_name;     /* name - index into section header string 
                   table section */ 
    Elf32_Word sh_type;     /* type */ 
    Elf32_Word sh_flags;    /* flags */ 
    Elf32_Addr sh_addr;     /* address */ 
    Elf32_Off sh_offset;    /* file offset */ 
    Elf32_Word sh_size;     /* section size */ 
    Elf32_Word sh_link;     /* section header table index link */ 
    Elf32_Word sh_info;     /* extra information */ 
    Elf32_Word sh_addralign;    /* address alignment */ 
    Elf32_Word sh_entsize;  /* section entry size */ 
} Elf32_Shdr; 
 
/* Special Section Indexes */ 
#define SHN_UNDEF   0       /* undefined */ 
#define SHN_LORESERVE   0xff00      /* lower bounds of reserved indexes */ 
#define SHN_LOPROC  0xff00      /* reserved range for processor */ 
#define SHN_HIPROC  0xff1f      /*   specific section indexes */ 
#define SHN_ABS     0xfff1      /* absolute value */ 
#define SHN_COMMON  0xfff2      /* common symbol */ 
#define SHN_HIRESERVE   0xffff      /* upper bounds of reserved indexes */ 
 
/* sh_type */ 
#define SHT_NULL    0       /* inactive */ 
#define SHT_PROGBITS    1       /* program defined information */ 
#define SHT_SYMTAB  2       /* symbol table section */ 
#define SHT_STRTAB  3       /* string table section */ 
#define SHT_RELA    4       /* relocation section with addends*/ 
#define SHT_HASH    5       /* symbol hash table section */ 
#define SHT_DYNAMIC 6       /* dynamic section */ 
#define SHT_NOTE    7       /* note section */ 
#define SHT_NOBITS  8       /* no space section */ 
#define SHT_REL     9       /* relation section without addends */ 
#define SHT_SHLIB   10      /* reserved - purpose unknown */ 
#define SHT_DYNSYM  11      /* dynamic symbol table section */ 
#define SHT_NUM     12      /* number of section types */ 
#define SHT_LOPROC  0x70000000  /* reserved range for processor */ 
#define SHT_HIPROC  0x7fffffff  /*  specific section header types */ 
#define SHT_LOUSER  0x80000000  /* reserved range for application */ 
#define SHT_HIUSER  0xffffffff  /*  specific indexes */ 
 
/* Section names */ 
#define ELF_BSS         ".bss"      /* uninitialized data */ 
#define ELF_DATA        ".data"     /* initialized data */ 
#define ELF_DEBUG       ".debug"    /* debug */ 
#define ELF_DYNAMIC     ".dynamic"  /* dynamic linking information */ 
#define ELF_DYNSTR      ".dynstr"   /* dynamic string table */ 
#define ELF_DYNSYM      ".dynsym"   /* dynamic symbol table */ 
#define ELF_FINI        ".fini"     /* termination code */ 
#define ELF_GOT         ".got"      /* global offset table */ 
#define ELF_HASH        ".hash"     /* symbol hash table */ 
#define ELF_INIT        ".init"     /* initialization code */ 
#define ELF_REL_DATA    ".rel.data" /* relocation data */ 
#define ELF_REL_FINI    ".rel.fini" /* relocation termination code */ 
#define ELF_REL_INIT    ".rel.init" /* relocation initialization code */ 
#define ELF_REL_DYN     ".rel.dyn"  /* relocaltion dynamic link info */ 
#define ELF_REL_RODATA  ".rel.rodata"   /* relocation read-only data */ 
#define ELF_REL_TEXT    ".rel.text" /* relocation code */ 
#define ELF_RODATA      ".rodata"   /* read-only data */ 
#define ELF_SHSTRTAB    ".shstrtab" /* section header string table */ 
#define ELF_STRTAB      ".strtab"   /* string table */ 
#define ELF_SYMTAB      ".symtab"   /* symbol table */ 
#define ELF_TEXT        ".text"     /* code */ 
 
 
/* Section Attribute Flags - sh_flags */ 
#define SHF_WRITE   	0x1     /* Writable */ 
#define SHF_ALLOC   	0x2     /* occupies memory */ 
#define SHF_EXECINSTR   0x4     /* executable */ 
#define SHF_MASKPROC    0xf0000000  /* reserved bits for processor */ 
                    /*  specific section attributes */ 
 
/* Symbol Table Entry */ 
typedef struct elf32_sym 
{ 
    Elf32_Word st_name;     /* name - index into string table */ 
    Elf32_Addr st_value;    /* symbol value */ 
    Elf32_Word st_size;     /* symbol size */ 
    unsigned char st_info;  /* type and binding */ 
    unsigned char st_other; /* 0 - no defined meaning */ 
    Elf32_Half st_shndx;    /* section header index */ 
} Elf32_Sym; 
 
/* Symbol table index */ 
#define STN_UNDEF   0       /* undefined */ 
 
/* Extract symbol info - st_info */ 
#define ELF32_ST_BIND(x)    ((x) >> 4) 
#define ELF32_ST_TYPE(x)    (((unsigned int) x) & 0xf) 
#define ELF32_ST_INFO(b,t)  (((b) << 4) + ((t) & 0xf)) 
 
/* Symbol Binding - ELF32_ST_BIND - st_info */ 
#define STB_LOCAL   0       /* Local symbol */ 
#define STB_GLOBAL  1       /* Global symbol */ 
#define STB_WEAK    2       /* like global - lower precedence */ 
#define STB_NUM     3       /* number of symbol bindings */ 
#define STB_LOPROC  13      /* reserved range for processor */ 
#define STB_HIPROC  15      /*  specific symbol bindings */ 
 
/* Symbol type - ELF32_ST_TYPE - st_info */ 
#define STT_NOTYPE  0       /* not specified */ 
#define STT_OBJECT  1       /* data object */ 
#define STT_FUNC    2       /* function */ 
#define STT_SECTION 3       /* section */ 
#define STT_FILE    4       /* file */ 
#define STT_NUM     5       /* number of symbol types */ 
#define STT_LOPROC  13      /* reserved range for processor */ 
#define STT_HIPROC  15      /*  specific symbol types */ 
 
/* Relocation entry with implicit addend */ 
typedef struct  
{ 
    Elf32_Addr r_offset;    /* offset of relocation */ 
    Elf32_Word r_info;      /* symbol table index and type */ 
} Elf32_Rel; 
 
/* Relocation entry with explicit addend */ 
typedef struct  
{ 
    Elf32_Addr r_offset;    /* offset of relocation */ 
    Elf32_Word r_info;      /* symbol table index and type */ 
    Elf32_Sword r_addend; 
} Elf32_Rela; 
 
/* Extract relocation info - r_info */ 
#define ELF32_R_SYM(i)      ((i) >> 8) 
#define ELF32_R_TYPE(i)     ((unsigned char) (i)) 
#define ELF32_R_INFO(s,t)   (((s) << 8) + (unsigned char)(t)) 
 
/* Program Header */ 
typedef struct 
{ 
    Elf32_Word p_type;      /* segment type */ 
    Elf32_Off p_offset;     /* segment offset */ 
    Elf32_Addr p_vaddr;     /* virtual address of segment */ 
    Elf32_Addr p_paddr;     /* physical address - ignored? */ 
    Elf32_Word p_filesz;    /* number of bytes in file for seg. */ 
    Elf32_Word p_memsz;     /* number of bytes in mem. for seg. */ 
    Elf32_Word p_flags;     /* flags */ 
    Elf32_Word p_align;     /* memory alignment */ 
} Elf32_Phdr; 
 
/* Segment types - p_type */ 
#define PT_NULL     0       /* unused */ 
#define PT_LOAD     1       /* loadable segment */ 
#define PT_DYNAMIC  2       /* dynamic linking section */ 
#define PT_INTERP   3       /* the RTLD */ 
#define PT_NOTE     4       /* auxiliary information */ 
#define PT_SHLIB    5       /* reserved - purpose undefined */ 
#define PT_PHDR     6       /* program header */ 
#define PT_NUM      7       /* Number of segment types */ 
#define PT_LOPROC   0x70000000  /* reserved range for processor */ 
#define PT_HIPROC   0x7fffffff  /*  specific segment types */ 
 
/* Segment flags - p_flags */ 
#define PF_X        0x1     /* Executable */ 
#define PF_W        0x2     /* Writable */ 
#define PF_R        0x4     /* Readable */ 
#define PF_MASKPROC 0xf0000000  /* reserved bits for processor */ 
                    /*  specific segment flags */ 
 
/* Dynamic structure */ 
typedef struct  
{ 
    Elf32_Sword d_tag;      /* controls meaning of d_val */ 
    union 
    { 
    Elf32_Word d_val;   /* Multiple meanings - see d_tag */ 
    Elf32_Addr d_ptr;   /* program virtual address */ 
    } d_un; 
} Elf32_Dyn; 
 
extern Elf32_Dyn    _DYNAMIC[]; 
 
/* Dynamic Array Tags - d_tag */ 
#define DT_NULL     0       /* marks end of _DYNAMIC array */ 
#define DT_NEEDED   1       /* string table offset of needed lib */ 
#define DT_PLTRELSZ 2       /* size of relocation entries in PLT */ 
#define DT_PLTGOT   3       /* address PLT/GOT */ 
#define DT_HASH     4       /* address of symbol hash table */ 
#define DT_STRTAB   5       /* address of string table */ 
#define DT_SYMTAB   6       /* address of symbol table */ 
#define DT_RELA     7       /* address of relocation table */ 
#define DT_RELASZ   8       /* size of relocation table */ 
#define DT_RELAENT  9       /* size of relocation entry */ 
#define DT_STRSZ    10      /* size of string table */ 
#define DT_SYMENT   11      /* size of symbol table entry */ 
#define DT_INIT     12      /* address of initialization func. */ 
#define DT_FINI     13      /* address of termination function */ 
#define DT_SONAME   14      /* string table offset of shared obj */ 
#define DT_RPATH    15      /* string table offset of library 
                      search path */ 
#define DT_SYMBOLIC 16      /* start sym search in shared obj. */ 
#define DT_REL      17      /* address of rel. tbl. w addends */ 
#define DT_RELSZ    18      /* size of DT_REL relocation table */ 
#define DT_RELENT   19      /* size of DT_REL relocation entry */ 
#define DT_PLTREL   20      /* PLT referenced relocation entry */ 
#define DT_DEBUG    21      /* bugger */ 
#define DT_TEXTREL  22      /* Allow rel. mod. to unwritable seg */ 
#define DT_JMPREL   23      /* add. of PLT's relocation entries */ 
#define DT_NUM      24      /* Number used. */ 
#define DT_LOPROC   0x70000000  /* reserved range for processor */ 
#define DT_HIPROC   0x7fffffff  /*  specific dynamic array tags */ 
 
/* Standard ELF hashing function */ 
unsigned long elf_hash(const unsigned char *name); 
 
#define ELF_TARG_VER    1   /* The ver for which this code is intended */ 
 
int do_swap = 0; 
 
static void elf_swap(char *mem, int len) 
{ 
	int t, i; 
 
    for (i = 0; i < len / 2; i++) 
    { 
        t = mem[i]; 
        mem[i] = mem[len - i - 1]; 
        mem[len - i - 1] = t; 
    } 
} 
 
void elf_dump_bin(char *data, unsigned long size) 
{ 
// fprintf(stderr, "dump: data=%p size=%#lx\n", data, size); 
	fwrite(data, sizeof *data, size, outfile); 

	#if 0
	if(verbose)
		fwrite(data, sizeof *data, size, stdout); 
	#endif
} 
 
/* dump a bunch of zero bytes to make sure that segments like bss 
  are initialized correctly 
*/ 
void elf_dump_zero(unsigned long size) 
{ 
// fprintf(stderr, "zero: size=%#lx\n", size); 
 
    while (size-- > 0) 
   	{
   	#if 0
		if(verbose)
	        putc('\0', stdout); 
	#endif
	
		fputc('\0', outfile); 
   	}
} 
 
void elf_swap_ehdr(Elf32_Ehdr *hdr) 
{ 
    if (do_swap) 
    { 
        elf_swap((char *)&hdr->e_type, sizeof hdr->e_type); 
        elf_swap((char *)&hdr->e_machine, sizeof hdr->e_machine); 
        elf_swap((char *)&hdr->e_version, sizeof hdr->e_version); 
        elf_swap((char *)&hdr->e_entry, sizeof hdr->e_entry); 
        elf_swap((char *)&hdr->e_phoff, sizeof hdr->e_phoff); 
        elf_swap((char *)&hdr->e_shoff, sizeof hdr->e_shoff); 
        elf_swap((char *)&hdr->e_flags, sizeof hdr->e_flags); 
        elf_swap((char *)&hdr->e_ehsize, sizeof hdr->e_ehsize); 
        elf_swap((char *)&hdr->e_phentsize, sizeof hdr->e_phentsize); 
        elf_swap((char *)&hdr->e_phnum, sizeof hdr->e_phnum); 
        elf_swap((char *)&hdr->e_shentsize, sizeof hdr->e_shentsize); 
        elf_swap((char *)&hdr->e_shnum, sizeof hdr->e_shnum); 
        elf_swap((char *)&hdr->e_shstrndx, sizeof hdr->e_shstrndx); 
    } 	


	if (verbose) 
	{ 
		printf("ELF2BIN>>  elf_swap_ehdr =============== \n");
		printf("e_type      : %u (0x%X) ", hdr->e_type, hdr->e_type ); 
		switch( hdr->e_type )
		{
			case ET_NONE: /* 파일의 타입이 없음 */
				printf("None type! \n");
				break;
			case ET_REL: /* 재배치 가능 파일 */
				printf("Relocation type! \n");
				break;
			case ET_EXEC: /* 실행 화일 */
				printf("Executable type! \n");
				break;
			case ET_DYN: /* 공유 object 파일 */
				printf("Sharing object type! \n");
				break;
			case ET_CORE: /* core 파일 */
				printf("Core file type! \n");
				break;
			case ET_LOPROC: /* 프로세서에 의존적인 파일 */
				printf("CPU depenenable type! \n");
				break;
			default:
				printf("unknown type! \n");
				break;
		}
		
		printf("e_machine   : %u (0x%X)\n", hdr->e_machine, hdr->e_machine ); 
		switch( hdr->e_machine )
		{
			case EM_NONE:
				break;
		}
		
		printf("e_version   : %u (0x%X)\n", hdr->e_version, hdr->e_version ); 
		printf("e_entry     : %u (0x%X)\n", hdr->e_entry, hdr->e_entry ); 
		printf("e_phoff     : %u (0x%X)\n", hdr->e_phoff, hdr->e_phoff ); 
		printf("e_shoff     : %u (0x%X)\n", hdr->e_shoff, hdr->e_shoff ); 
		printf("e_flags     : %u (0x%X)\n", hdr->e_flags, hdr->e_flags ); 
		printf("e_ehsize    : %u (0x%X)\n", hdr->e_ehsize, hdr->e_ehsize ); 
		printf("e_phentsize : %u (0x%X)\n", hdr->e_phentsize, hdr->e_phentsize ); 
		printf("e_phnum     : %u (0x%X)\n", hdr->e_phnum, hdr->e_phnum ); 
		printf("e_shentsize : %u (0x%X)\n", hdr->e_shentsize, hdr->e_shentsize ); 
		printf("e_shnum     : %u (0x%X)\n", hdr->e_shnum, hdr->e_shnum ); 
		printf("e_shstrndx  : %u (0x%X)\n", hdr->e_shstrndx, hdr->e_shstrndx ); 
	} 


} 
 
void elf_swap_phdr(Elf32_Phdr *p) 
{ 
    if (do_swap) 
    { 
        elf_swap((char *)&p->p_type, sizeof p->p_type); 
        elf_swap((char *)&p->p_offset, sizeof p->p_offset); 
        elf_swap((char *)&p->p_vaddr, sizeof p->p_vaddr); 
        elf_swap((char *)&p->p_paddr, sizeof p->p_paddr); 
        elf_swap((char *)&p->p_filesz, sizeof p->p_filesz); 
        elf_swap((char *)&p->p_memsz, sizeof p->p_memsz); 
        elf_swap((char *)&p->p_flags, sizeof p->p_flags); 
        elf_swap((char *)&p->p_align, sizeof p->p_align); 

		if (verbose) 
		{ 
			printf("ELF2BIN>>  elf_swap_phdr =============== \n");
			printf("p_type   : %u (0x%X)\n", p->p_type, p->p_type ); 
			printf("p_offset : %u (0x%X)\n", p->p_offset, p->p_offset ); 
			printf("p_vaddr  : %u (0x%X)\n", p->p_vaddr, p->p_vaddr ); 
			printf("p_paddr  : %u (0x%X)\n", p->p_paddr, p->p_paddr ); 
			printf("p_filesz : %u (0x%X)\n", p->p_filesz, p->p_filesz ); 
			printf("p_memsz  : %u (0x%X)\n", p->p_memsz, p->p_memsz ); 
			printf("p_flags  : %u (0x%X)\n", p->p_flags, p->p_flags ); 
			printf("p_align	 : %u (0x%X)\n", p->p_align, p->p_align ); 
		} 

    } 
} 
 
void elf_swap_shdr(Elf32_Shdr *s) 
{ 
    if (do_swap) 
    { 
        elf_swap((char *)&s->sh_name, sizeof s->sh_name); 
        elf_swap((char *)&s->sh_type, sizeof s->sh_type); 
        elf_swap((char *)&s->sh_flags, sizeof s->sh_flags); 
        elf_swap((char *)&s->sh_addr, sizeof s->sh_addr); 
        elf_swap((char *)&s->sh_offset, sizeof s->sh_offset); 
        elf_swap((char *)&s->sh_size, sizeof s->sh_size); 
        elf_swap((char *)&s->sh_link, sizeof s->sh_link); 
        elf_swap((char *)&s->sh_info, sizeof s->sh_info); 
        elf_swap((char *)&s->sh_addralign, sizeof s->sh_addralign); 
        elf_swap((char *)&s->sh_entsize, sizeof s->sh_entsize); 

		if (verbose) 
		{ 
			printf("ELF2BIN>>  elf_swap_shdr =============== \n");
			printf("sh_name      : %u (0x%X)\n", s->sh_name, s->sh_name ); 
			printf("sh_type      : %u (0x%X)\n", s->sh_type, s->sh_type ); 
			printf("sh_flags     : %u (0x%X)\n", s->sh_flags, s->sh_flags ); 
			printf("sh_addr      : %u (0x%X)\n", s->sh_addr, s->sh_addr ); 
			printf("sh_offset    : %u (0x%X)\n", s->sh_offset, s->sh_offset ); 
			printf("sh_size      : %u (0x%X)\n", s->sh_size, s->sh_size ); 
			printf("sh_link      : %u (0x%X)\n", s->sh_link, s->sh_link ); 
			printf("sh_info      : %u (0x%X)\n", s->sh_info, s->sh_info ); 
			printf("sh_addralign : %u (0x%X)\n", s->sh_addralign, s->sh_addralign ); 
			printf("sh_entsize   : %u (0x%X)\n", s->sh_entsize, s->sh_entsize ); 
		} 


    } 
} 
 
int elf_dumpimage(char *image, int imagelen) 
{ 
    Elf32_Ehdr *hdr = (Elf32_Ehdr *)image; 
    Elf32_Shdr *scnhdr; 
    Elf32_Phdr *proghdr, *p; 
    Elf32_Sym *syms; 
    int i, l; 
    unsigned long last = 0; 
    Elf32_Ehdr ehdr; 
    Elf32_Shdr shdr; 
    Elf32_Shdr *symtab = NULL; 
    Elf32_Shdr *strtab = NULL; 
    Elf32_Shdr *shstrtab = NULL; 
 
    if (!IS_ELF(*hdr)) 
    { 
        printf("ELF2BIN>> It is not ELF binary.\n"); 

		if(data_buf){ free(data_buf);	data_buf=NULL; } // 2020.07.17
        return 10; 
    } 
 	else
	{
 		if(verbose)
		{
			printf("ELF2BIN>> file ID       :[0x%X %X %X %X] \n", 
				hdr->e_ident[EI_MAG0], hdr->e_ident[EI_MAG1], hdr->e_ident[EI_MAG2], hdr->e_ident[EI_MAG3] ); 	
			printf("ELF2BIN>> file class    :[0x%X] ", hdr->e_ident[EI_CLASS] );	
			switch( hdr->e_ident[EI_CLASS] )
			{
				case ELFCLASSNONE: /* Invalid class */
					printf(" Invalid class \n" );	
					break;
				case ELFCLASS32: /* 32 bit objects */
					printf(" 32bit objects\n" );	
					break;
				case ELFCLASS64: /* 64 bit objects */
					printf(" 64bit objects\n" );	
					break;
				case ELFCLASSNUM: /* ELF class number */
					printf(" ELF class number \n" );	
					break;
				default:
					printf(" unknown \n" );	
					break;
			}
			printf("ELF2BIN>> data encoding :[0x%X] (%s) \n", hdr->e_ident[EI_DATA],
					( ELFDATA2LSB==hdr->e_ident[EI_DATA]? "Little-endian" : 
						(ELFDATA2MSB==hdr->e_ident[EI_DATA]? "Big-endian" : "invalid") )  );
			printf("ELF2BIN>> hdr version   :[0x%X] \n", hdr->e_ident[EI_VERSION] );	
		}
	}

	
#ifdef ELF_LITTLE_ENDIAN 
    do_swap = (hdr->e_ident[EI_DATA] == ELFDATA2MSB); 
#else 
    do_swap = (hdr->e_ident[EI_DATA] == ELFDATA2LSB); 
#endif 
 
    elf_swap_ehdr(hdr); 
    proghdr = (Elf32_Phdr *)(image + hdr->e_phoff); 
 
    for (i = 0; i < hdr->e_phnum; i++) 
    { 
        p = &proghdr[i]; 
        elf_swap_phdr(p); 
 
        if (p->p_type != PT_LOAD) 
            continue; 

 		if(verbose)
			printf("ELF2BIN>> %02d: vaddr=%#x memsz=%#x filesz=%#x\n", i, p->p_vaddr, p->p_memsz, p->p_filesz); 
 
        if (last > 0 && last < p->p_vaddr) 
            elf_dump_zero(p->p_vaddr - last); 
 
        elf_dump_bin(image + p->p_offset, p->p_filesz); 
 
        if (p->p_memsz > p->p_filesz) 
            elf_dump_zero(p->p_memsz - p->p_filesz); 
 
        last = p->p_vaddr + p->p_memsz; 
    } 
 
    scnhdr = (Elf32_Shdr *)(image + hdr->e_shoff); 
 
    for (i = 0; i < hdr->e_shnum; i++) 
        elf_swap_shdr(&scnhdr[i]); 
 
    shstrtab = &scnhdr[hdr->e_shstrndx]; 
 
    for (i = 0; i < hdr->e_shnum; i++) 
    { 
        if (scnhdr[i].sh_type != SHT_SYMTAB || scnhdr[i].sh_offset == 0) 
            continue; 
 
        symtab = &scnhdr[i]; 
        strtab = &scnhdr[scnhdr[i].sh_link]; 
 
        if (strtab->sh_type != SHT_STRTAB) 
            fprintf(stderr, "ELF2BIN>> section %d should be strtab?!?\n", 
                    scnhdr[i].sh_link); 
 
        break; 
    } 
 
    if (symtab == NULL || strtab == NULL || shstrtab == NULL || 
            symtab->sh_entsize != sizeof *syms) 
    { 
        printf("ELF2BIN>> no symbol table?!?\n"); 

		if(data_buf){ free(data_buf);	data_buf=NULL; } // 2020.07.17
        return 0; 
    } 
 
    ehdr = *hdr; 
    ehdr.e_phnum = 0; 
    ehdr.e_phoff = 0; 
    ehdr.e_shnum = 4; 
    ehdr.e_shoff = sizeof (*hdr); 
    ehdr.e_shstrndx = 3; 

	if(verbose)
		printf("ELF2BIN>> dump syms at file offset %#x...\n", ftell(outfile)); 
	
    elf_swap_ehdr(&ehdr); 
    elf_dump_bin((char *)&ehdr, sizeof ehdr); 
    elf_swap_ehdr(&ehdr); 
 
    memset(&shdr, 0, sizeof shdr); 
    elf_swap_shdr(&shdr); 
    elf_dump_bin((char *)&shdr, sizeof shdr); 
 
    i = ehdr.e_shoff + ehdr.e_shentsize * ehdr.e_shnum; 
    shdr = *symtab; 
    shdr.sh_offset = i; 
    shdr.sh_link = 2; 
    elf_swap_shdr(&shdr); 
    elf_dump_bin((char *)&shdr, sizeof shdr); 
 
    shdr = *strtab; 
    shdr.sh_offset = i + symtab->sh_size; 
    elf_swap_shdr(&shdr); 
    elf_dump_bin((char *)&shdr, sizeof shdr); 
 
    shdr = *shstrtab; 
    shdr.sh_offset = i + symtab->sh_size + strtab->sh_size; 
    elf_swap_shdr(&shdr); 
    elf_dump_bin((char *)&shdr, sizeof shdr); 
 
    elf_dump_bin(image + symtab->sh_offset, symtab->sh_size); 
    elf_dump_bin(image + strtab->sh_offset, strtab->sh_size); 
    elf_dump_bin(image + shstrtab->sh_offset, shstrtab->sh_size); 
 
    return 0; 
} 
 
#endif /// ELF2BIN






