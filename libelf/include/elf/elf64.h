/* @LICENSE(UNSW_OZPLB) */

/*
 * Australian Public Licence B (OZPLB)
 *
 * Version 1-0
 *
 * Copyright (c) 2004 University of New South Wales
 *
 * All rights reserved.
 *
 * Developed by: Operating Systems and Distributed Systems Group (DiSy)
 *               University of New South Wales
 *               http://www.disy.cse.unsw.edu.au
 *
 * Permission is granted by University of New South Wales, free of charge, to
 * any person obtaining a copy of this software and any associated
 * documentation files (the "Software") to deal with the Software without
 * restriction, including (without limitation) the rights to use, copy,
 * modify, adapt, merge, publish, distribute, communicate to the public,
 * sublicense, and/or sell, lend or rent out copies of the Software, and
 * to permit persons to whom the Software is furnished to do so, subject
 * to the following conditions:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimers.
 *
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimers in the documentation and/or other materials provided
 *       with the distribution.
 *
 *     * Neither the name of University of New South Wales, nor the names of its
 *       contributors, may be used to endorse or promote products derived
 *       from this Software without specific prior written permission.
 *
 * EXCEPT AS EXPRESSLY STATED IN THIS LICENCE AND TO THE FULL EXTENT
 * PERMITTED BY APPLICABLE LAW, THE SOFTWARE IS PROVIDED "AS-IS", AND
 * NATIONAL ICT AUSTRALIA AND ITS CONTRIBUTORS MAKE NO REPRESENTATIONS,
 * WARRANTIES OR CONDITIONS OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO ANY REPRESENTATIONS, WARRANTIES OR CONDITIONS
 * REGARDING THE CONTENTS OR ACCURACY OF THE SOFTWARE, OR OF TITLE,
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, NONINFRINGEMENT,
 * THE ABSENCE OF LATENT OR OTHER DEFECTS, OR THE PRESENCE OR ABSENCE OF
 * ERRORS, WHETHER OR NOT DISCOVERABLE.
 *
 * TO THE FULL EXTENT PERMITTED BY APPLICABLE LAW, IN NO EVENT SHALL
 * NATIONAL ICT AUSTRALIA OR ITS CONTRIBUTORS BE LIABLE ON ANY LEGAL
 * THEORY (INCLUDING, WITHOUT LIMITATION, IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHERWISE) FOR ANY CLAIM, LOSS, DAMAGES OR OTHER
 * LIABILITY, INCLUDING (WITHOUT LIMITATION) LOSS OF PRODUCTION OR
 * OPERATION TIME, LOSS, DAMAGE OR CORRUPTION OF DATA OR RECORDS; OR LOSS
 * OF ANTICIPATED SAVINGS, OPPORTUNITY, REVENUE, PROFIT OR GOODWILL, OR
 * OTHER ECONOMIC LOSS; OR ANY SPECIAL, INCIDENTAL, INDIRECT,
 * CONSEQUENTIAL, PUNITIVE OR EXEMPLARY DAMAGES, ARISING OUT OF OR IN
 * CONNECTION WITH THIS LICENCE, THE SOFTWARE OR THE USE OF OR OTHER
 * DEALINGS WITH THE SOFTWARE, EVEN IF NATIONAL ICT AUSTRALIA OR ITS
 * CONTRIBUTORS HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH CLAIM, LOSS,
 * DAMAGES OR OTHER LIABILITY.
 *
 * If applicable legislation implies representations, warranties, or
 * conditions, or imposes obligations or liability on University of New South
 * Wales or one of its contributors in respect of the Software that
 * cannot be wholly or partly excluded, restricted or modified, the
 * liability of University of New South Wales or the contributor is limited, to
 * the full extent permitted by the applicable legislation, at its
 * option, to:
 * a.  in the case of goods, any one or more of the following:
 * i.  the replacement of the goods or the supply of equivalent goods;
 * ii.  the repair of the goods;
 * iii. the payment of the cost of replacing the goods or of acquiring
 *  equivalent goods;
 * iv.  the payment of the cost of having the goods repaired; or
 * b.  in the case of services:
 * i.  the supplying of the services again; or
 * ii.  the payment of the cost of having the services supplied again.
 *
 * The construction, validity and performance of this licence is governed
 * by the laws in force in New South Wales, Australia.
 */
#pragma once

#include <stdint.h>
#include <elf/elf.h>

/*
 * File header
 */
struct Elf64_Header {
    unsigned char   e_ident[16];
    uint16_t    e_type;         /* Relocatable=1, Executable=2 (+ some more ..) */
    uint16_t    e_machine;      /* Target architecture: MIPS=8 */
    uint32_t    e_version;      /* Elf version (should be 1) */
    uint64_t    e_entry;        /* Code entry point */
    uint64_t    e_phoff;        /* Program header table */
    uint64_t    e_shoff;        /* Section header table */
    uint32_t    e_flags;        /* Flags */
    uint16_t    e_ehsize;       /* ELF header size */
    uint16_t    e_phentsize;    /* Size of one program segment header */
    uint16_t    e_phnum;        /* Number of program segment headers */
    uint16_t    e_shentsize;    /* Size of one section header */
    uint16_t    e_shnum;        /* Number of section headers */
    uint16_t    e_shstrndx;     /* Section header index of the string table
                                   for section header names */
};

/*
 * Section header
*/
struct Elf64_Shdr {
    uint32_t    sh_name;
    uint32_t    sh_type;
    uint64_t    sh_flags;
    uint64_t    sh_addr;
    uint64_t    sh_offset;
    uint64_t    sh_size;
    uint32_t    sh_link;
    uint32_t    sh_info;
    uint64_t    sh_addralign;
    uint64_t    sh_entsize;
};

/*
 * Program header
 */
struct Elf64_Phdr {
    uint32_t    p_type;     /* Segment type: Loadable segment = 1 */
    uint32_t    p_flags;    /* Flags: logical "or" of PF_* constants below */
    uint64_t    p_offset;   /* Offset of segment in file */
    uint64_t    p_vaddr;    /* Reqd virtual address of segment when loading */
    uint64_t    p_paddr;    /* Reqd physical address of segment */
    uint64_t    p_filesz;   /* How many bytes this segment occupies in file */
    uint64_t    p_memsz;    /* How many bytes this segment should occupy in memory
                               (when loading, expand the segment by concatenating
                               enough zero bytes to it) */
    uint64_t    p_align;    /* Reqd alignment of segment in memory */
};

/*
 * Dynamic section
 */
struct Elf64_Dyn {
    uint64_t d_tag;
    union {
        uint64_t d_val;
        uint64_t d_ptr;
    } d_un;
};

/* ELF header functions */
int elf64_checkFile(elf_t *elf);

int elf64_checkProgramHeaderTable(elf_t *elf);

int elf64_checkSectionTable(elf_t *elf);

static inline bool
elf_isElf64(elf_t *elf)
{
    return elf->elfClass == ELFCLASS64;
}

static inline struct Elf64_Header
elf64_getHeader(elf_t *elf)
{
    return *(struct Elf64_Header *) elf->elfFile;
}

static inline uint64_t
elf64_getEntryPoint(elf_t *file)
{
    return elf64_getHeader(file).e_entry;
}

static inline struct Elf64_Phdr *
elf64_getProgramHeaderTable(elf_t *file)
{
    return file->elfFile + elf64_getHeader(file).e_phoff;
}

static inline struct Elf64_Shdr *
elf64_getSectionTable(elf_t *file)
{
    return file->elfFile + elf64_getHeader(file).e_shoff;
}

static inline uint16_t
elf64_getNumProgramHeaders(elf_t *file)
{
    return elf64_getHeader(file).e_phnum;
}

static inline unsigned
elf64_getNumSections(elf_t *elf) {
    return elf64_getHeader(elf).e_shnum;
}

static inline uint16_t
elf64_getSectionStringTableIndex(elf_t *elf)
{
    return elf64_getHeader(elf).e_shstrndx;
}

char *elf64_getStringTable(elf_t *elf, int string_segment);

char *elf64_getSectionStringTable(elf_t *elf);

/* Section header functions */
void *elf64_getSection(elf_t *elf, int i);

void *elf64_getSectionNamed(elf_t *elf, const char *str, int *i);

char *elf64_getSectionName(elf_t *elf, int i);

static inline uint32_t
elf64_getSectionNameOffset(elf_t *elf, uint16_t s)
{
    return elf64_getSectionTable(elf)[s].sh_name;
}

static inline uint32_t
elf64_getSectionType(elf_t *file, uint16_t s)
{
    return elf64_getSectionTable(file)[s].sh_type;
}

static inline uint32_t
elf64_getSectionFlags(elf_t *file, uint16_t s)
{
    return elf64_getSectionTable(file)[s].sh_flags;
}

static inline uint64_t
elf64_getSectionAddr(elf_t *elf, int i)
{
    return elf64_getSectionTable(elf)[i].sh_addr;
}

static inline uint64_t
elf64_getSectionOffset(elf_t *elf, int i)
{
    return elf64_getSectionTable(elf)[i].sh_offset;
}

static inline uint64_t
elf64_getSectionSize(void *elf, int i)
{
    return elf64_getSectionTable(elf)[i].sh_size;
}

/* Program header functions */
void *elf64_getProgramSegment(elf_t *elf, uint16_t ph);

static inline uint32_t
elf64_getProgramHeaderType(elf_t *file, uint16_t ph)
{
    return elf64_getProgramHeaderTable(file)[ph].p_type;
}

static inline uint64_t
elf64_getProgramHeaderOffset(elf_t *file, uint16_t ph)
{
    return elf64_getProgramHeaderTable(file)[ph].p_offset;
}

static inline uint64_t
elf64_getProgramHeaderVaddr(elf_t *file, uint16_t ph)
{
    return elf64_getProgramHeaderTable(file)[ph].p_vaddr;
}

static inline uint64_t
elf64_getProgramHeaderPaddr(elf_t *file, uint16_t ph)
{
    return elf64_getProgramHeaderTable(file)[ph].p_paddr;
}

static inline uint64_t
elf64_getProgramHeaderFileSize(elf_t *file, uint16_t ph)
{
    return elf64_getProgramHeaderTable(file)[ph].p_filesz;
}

static inline uint64_t
elf64_getProgramHeaderMemorySize(elf_t *file, uint16_t ph)
{
    return elf64_getProgramHeaderTable(file)[ph].p_memsz;
}

static inline uint32_t
elf64_getProgramHeaderFlags(elf_t *file, uint16_t ph)
{
    return elf64_getProgramHeaderTable(file)[ph].p_flags;
}

static inline uint32_t
elf64_getProgramHeaderAlign(elf_t *file, uint16_t ph)
{
    return elf64_getProgramHeaderTable(file)[ph].p_align;
}

