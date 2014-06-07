#include "defines.h"
#include "elf.h"
#include "lib.h"
#include "peripheralInterface.h"

struct elf_header
{
  struct
  {
    uint8 magic[4];
    /* 32bit/64bit */
    uint8 class;
    /* エンディアン(readelfではDataと表記される) */
    uint8 format;
    /* ELF形式のバージョン */
    uint8 version;
    /* OSの種別(OS/ABI) */
    uint8 abi;
    /* OSのバージョン(ABI Version) */
    uint8 abi_version;
    /* 予約(未使用) */
    uint8 reserve[7];
  } id;

  int16 type; /* 種別(オブジェクトファイル,実行形式ファイルなど) */
  int16 arch; /* CPUアーキテクチャ(h8,i386など) */
  int32 version; /* ELF形式のバージョン */
  int32 entry_point; /* 実行開始アドレス(エントリ・ポイント) */
  int32 program_header_offset; /* プログラムヘッダテーブルの位置 */
  int32 section_header_offset; /* セクションヘッダテーブルの位置 */
  int32 flags; /* フラグ */
  int16 header_size; /* ELFヘッダのサイズ */
  int16 program_header_size; /* プログラムヘッダのサイズ */
  int16 program_header_number; /* プログラムヘッダの個数 */
  int16 section_header_size; /* セクションヘッダのサイズ */
  int16 section_header_number; /* セクションヘッダの個数 */
  int16 section_name_index; /* セクション名を格納するセクションの番号 */
};

struct elf_program_header
{
  int32 type; /* セグメントの種別 */
  int32 offset; /* セグメントのELFファイル上での位置 */
  int32 virturl_address; /* セグメントを展開する論理アドレス(VA) */
  int32 physical_address; /* セグメントを展開する物理アドレス(PA) */
  int32 file_size; /* セグメントのELFファイル上でのサイズ */
  int32 memory_size; /* セグメントのメモリ上でのサイズ */
  int32 flags; /* 各種フラグ */
  int32 align; /* セグメントのアラインメント */
};

static bool
magic_number_is_ELF (struct elf_header *header)
{
  if (memory_compare (header->id.magic, "\x7f" "ELF", 4))
    return 0;
  return 1;
}

static bool
class_is_32bit (struct elf_header *header)
{
  if (header->id.class != 1)
    return 0;
  return 1;
}

static bool
format_is_big_endian (struct elf_header *header)
{
  if (header->id.format != 2)
    return 0;
  return 1;
}

static bool
EI_version_is_1 (struct elf_header *header)
{
  if (header->id.version != 1)
    return 0;
  return 1;
}

static bool
type_is_executable_file (struct elf_header *header)
{
  if (header->type != 2)
    return 0;
  return 1;
}

static bool
e_version_is_1 (struct elf_header *header)
{
  if (header->version != 1)
    return 0;
  return 1;
}

static bool
arch_is_h8 (struct elf_header *header)
{
  if ((header->arch != 46) && (header->arch != 47))
    return 0;
  return 1;
}

static int16
elf_check (struct elf_header *header)
{
  if (magic_number_is_ELF (header) & class_is_32bit (header)
      & format_is_big_endian (header) & EI_version_is_1 (header)
      & type_is_executable_file (header) & e_version_is_1 (header)
      & arch_is_h8 (header))
    return 0;
  return -1;
}

static bool
segment_type_is_LOAD (struct elf_program_header *header)
{
  if (header->type != 1)
    return 0;
  return 1;
}

static int16
elf_load_program (struct elf_header *header)
{
  int16 i;
  struct elf_program_header *program_header;

  for (i = 0; i < header->program_header_number; i++)
    {
      program_header = (struct elf_program_header *) ((int8 *) header
          + header->program_header_offset + header->program_header_size * i);
      if (!segment_type_is_LOAD (program_header))
        continue;
      /* .textと.rodataを展開 */
      memory_data_copy ((int8 *) program_header->physical_address,
                        (int8 *) header + program_header->offset,
                        program_header->file_size);
      /* .dataと.bssを展開 */
      set_data_in_memory (
          (int8 *) program_header->physical_address + program_header->file_size,
          0, program_header->memory_size - program_header->file_size);
    }
  return 0;
}

int8 *
elf_load (int8 *buffer_start_address)
{
  struct elf_header *header = (struct elf_header *) buffer_start_address;

  if (elf_check (header) < 0)
    return NULL;
  if (elf_load_program (header) < 0)
    return NULL;

  return (char *) header->entry_point;
}
