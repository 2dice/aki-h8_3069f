#include "defines.h"
#include "elf.h"
#include "lib.h"
#include "peripheralInterface.h"

struct elf_header
{
  struct{
    uint8 magic[4];
    uint8 class;               /* 32bit/64bit */
    uint8 format;              /* エンディアン(readelfではDataと表記される) */
    uint8 version;             /* ELF形式のバージョン */
    uint8 abi;                 /* OSの種別(OS/ABI) */
    uint8 abi_version;         /* OSのバージョン(ABI Version) */
    uint8 reserve[7];          /* 予約(未使用) */
  }id;

  int16 type;                  /* 種別(オブジェクトファイル,実行形式ファイルなど) */
  int16 arch;                  /* CPUアーキテクチャ(h8,i386など) */
  int32 version;               /* ELF形式のバージョン */
  int32 entry_point;           /* 実行開始アドレス(エントリ・ポイント) */
  int32 program_header_offset; /* プログラムヘッダテーブルの位置 */
  int32 section_header_offset; /* セクションヘッダテーブルの位置 */
  int32 flags;                 /* フラグ */
  int16 header_size;           /* ELFヘッダのサイズ */
  int16 program_header_size;   /* プログラムヘッダのサイズ */
  int16 program_header_number; /* プログラムヘッダの個数 */
  int16 section_header_size;   /* セクションヘッダのサイズ */
  int16 section_header_number; /* セクションヘッダの個数 */
  int16 section_name_index;    /* セクション名を格納するセクションの番号 */
};

struct elf_program_header
{
  int32 type;                  /* セグメントの種別 */
  int32 offset;                /* セグメントのELFファイル上での位置 */
  int32 virturl_address;       /* セグメントの論理アドレス(VA) */
  int32 physical_address;      /* セグメントの物理アドレス(PA) */
  int32 file_size;             /* セグメントのELFファイル上でのサイズ */
  int32 memory_size;           /* セグメントのメモリ上でのサイズ */
  int32 flags;                 /* 各種フラグ */
  int32 align;                 /* セグメントのアラインメント */
};

static int16 elf_check(struct elf_header *header)
{
/* TODO:関数かdefineで切り離す */
  if(memory_compare(header -> id.magic, "\x7f" "ELF", 4))
    return -1;                            /* マジックナンバーがELF形式かどうか確認 */
  if(header -> id.class != 1) return -1;  /* 32bitかどうか確認 */
  if(header -> id.format != 2) return -1; /* ビッグエンディアンかどうか確認 */
  if(header -> id.version != 1) return -1;/* バージョンが1かどうか確認 */
  if(header -> type != 2) return -1;      /* executable fileかどうか確認 */
  if(header -> version != 1) return -1;   /* バージョンが1かどうか確認 */
  /* アーキテクチャがh8かどうか確認 */
  if((header -> arch != 46) && (header -> arch !=47)) return -1;

  return 0;
}

static int16 elf_load_program(struct elf_header *header)
{
  int16 i;
  struct elf_program_header *program_header;
      
  for(i = 0; i < header->program_header_number; i++)
  {
    program_header = (struct elf_program_header *)
        ((int8 *)header + header ->program_header_offset +
         header -> program_header_size * i);
    if(program_header -> type != 1)
      continue;
    put_hex(program_header -> offset, 6);
    put_string(" ");
    put_hex(program_header -> virturl_address, 8);
    put_string(" ");
    put_hex(program_header -> physical_address, 8);
    put_string(" ");
    put_hex(program_header -> file_size, 5);
    put_string(" ");
    put_hex(program_header -> memory_size, 5);
    put_string(" ");
    put_hex(program_header -> flags, 2);
    put_string(" ");
    put_hex(program_header -> align, 2);
    put_string("\n");
  }
  return 0;
}

int16 elf_load(int8 *buf)
{
  struct elf_header *header = (struct elf_header *)buf;

  if(elf_check(header) < 0)
    return -1;
  if(elf_load_program(header) < 0)
    return -1;

  return 0;
}
