// REQUIRES: ppc32-registered-target,ppc64-registered-target,mips-registered-target
// RUN: %clang -no-canonical-prefixes \
// RUN:   -target powerpc-pc-freebsd8 %s    \
// RUN:   --sysroot=%S/Inputs/basic_freebsd_tree -### 2>&1 \
// RUN:   | FileCheck --check-prefix=CHECK-PPC %s
// CHECK-PPC: clang{{.*}}" "-cc1" "-triple" "powerpc-pc-freebsd8"
// CHECK-PPC: ld{{.*}}" "--sysroot=[[SYSROOT:[^"]+]]"
// CHECK-PPC: "--eh-frame-hdr" "-dynamic-linker" "{{.*}}ld-elf{{.*}}" "-o" "a.out" "{{.*}}crt1.o" "{{.*}}crti.o" "{{.*}}crtbegin.o" "-L[[SYSROOT]]/usr/lib" "{{.*}}.o" "-lgcc" "--as-needed" "-lgcc_s" "--no-as-needed" "-lc" "-lgcc" "--as-needed" "-lgcc_s" "--no-as-needed" "{{.*}}crtend.o" "{{.*}}crtn.o"
//
// RUN: %clang -no-canonical-prefixes \
// RUN:   -target powerpc64-pc-freebsd8 %s                              \
// RUN:   --sysroot=%S/Inputs/basic_freebsd64_tree -### 2>&1 \
// RUN:   | FileCheck --check-prefix=CHECK-PPC64 %s
// CHECK-PPC64: clang{{.*}}" "-cc1" "-triple" "powerpc64-pc-freebsd8"
// CHECK-PPC64: ld{{.*}}" "--sysroot=[[SYSROOT:[^"]+]]"
// CHECK-PPC64: "--eh-frame-hdr" "-dynamic-linker" "{{.*}}ld-elf{{.*}}" "-o" "a.out" "{{.*}}crt1.o" "{{.*}}crti.o" "{{.*}}crtbegin.o" "-L[[SYSROOT]]/usr/lib" "{{.*}}.o" "-lgcc" "--as-needed" "-lgcc_s" "--no-as-needed" "-lc" "-lgcc" "--as-needed" "-lgcc_s" "--no-as-needed" "{{.*}}crtend.o" "{{.*}}crtn.o"
//
//
// Check that -m32 properly adjusts the toolchain flags.
//
// RUN: %clang -no-canonical-prefixes -target x86_64-pc-freebsd8 -m32 %s \
// RUN:   --sysroot=%S/Inputs/multiarch_freebsd64_tree -### 2>&1 \
// RUN:   | FileCheck --check-prefix=CHECK-LIB32 %s
// CHECK-LIB32: clang{{.*}}" "-cc1" "-triple" "i386-pc-freebsd8"
// CHECK-LIB32: ld{{.*}}" {{.*}} "-m" "elf_i386_fbsd"
//
// RUN: %clang -target x86_64-pc-freebsd8 -m32 %s 2>&1 \
// RUN:   --sysroot=%S/Inputs/multiarch_freebsd64_tree -print-search-dirs 2>&1 \
// RUN:   | FileCheck --check-prefix=CHECK-LIB32PATHS %s
// CHECK-LIB32PATHS: libraries: ={{.*:?}}/usr/lib32
//
// Check that the new linker flags are passed to FreeBSD
// RUN: %clang -no-canonical-prefixes -target x86_64-pc-freebsd8 -m32 %s \
// RUN:   --sysroot=%S/Inputs/multiarch_freebsd64_tree -### 2>&1 \
// RUN:   | FileCheck --check-prefix=CHECK-LDFLAGS8 %s
// RUN: %clang -no-canonical-prefixes -target x86_64-pc-freebsd9 -m32 %s \
// RUN:   --sysroot=%S/Inputs/multiarch_freebsd64_tree -### 2>&1 \
// RUN:   | FileCheck --check-prefix=CHECK-LDFLAGS9 %s
// RUN: %clang -no-canonical-prefixes -target x86_64-pc-freebsd10.0 -m32 %s \
// RUN:   --sysroot=%S/Inputs/multiarch_freebsd64_tree -### 2>&1 \
// RUN:   | FileCheck --check-prefix=CHECK-LDFLAGS9 %s
// CHECK-LDFLAGS8-NOT: --hash-style=both
// CHECK-LDFLAGS8: --enable-new-dtags
// CHECK-LDFLAGS9: --hash-style=both
// CHECK-LDFLAGS9: --enable-new-dtags
//
// Check that we do not pass --hash-style=gnu and --hash-style=both to linker
// and provide correct path to the dynamic linker for MIPS platforms.
// Also verify that we tell the assembler to target the right ISA and ABI.
// RUN: %clang %s -### -o %t.o 2>&1 \
// RUN:     -target mips-unknown-freebsd10.0 \
// RUN:   | FileCheck --check-prefix=CHECK-MIPS %s
// CHECK-MIPS: "{{[^" ]*}}ld{{[^" ]*}}"
// CHECK-MIPS: "-dynamic-linker" "{{.*}}/libexec/ld-elf.so.1"
// CHECK-MIPS-NOT: "--hash-style={{gnu|both}}"
// RUN: %clang %s -### -o %t.o 2>&1 \
// RUN:     -target mipsel-unknown-freebsd10.0 \
// RUN:   | FileCheck --check-prefix=CHECK-MIPSEL %s
// CHECK-MIPSEL: "{{[^" ]*}}ld{{[^" ]*}}"
// CHECK-MIPSEL: "-dynamic-linker" "{{.*}}/libexec/ld-elf.so.1"
// CHECK-MIPSEL-NOT: "--hash-style={{gnu|both}}"
// RUN: %clang %s -### -o %t.o 2>&1 \
// RUN:     -target mips64-unknown-freebsd10.0 \
// RUN:   | FileCheck --check-prefix=CHECK-MIPS64 %s
// CHECK-MIPS64: "{{[^" ]*}}ld{{[^" ]*}}"
// CHECK-MIPS64: "-dynamic-linker" "{{.*}}/libexec/ld-elf.so.1"
// CHECK-MIPS64-NOT: "--hash-style={{gnu|both}}"
// RUN: %clang %s -### -o %t.o 2>&1 \
// RUN:     -target mips64el-unknown-freebsd10.0 \
// RUN:   | FileCheck --check-prefix=CHECK-MIPS64EL %s
// CHECK-MIPS64EL: "{{[^" ]*}}ld{{[^" ]*}}"
// CHECK-MIPS64EL: "-dynamic-linker" "{{.*}}/libexec/ld-elf.so.1"
// CHECK-MIPS64EL-NOT: "--hash-style={{gnu|both}}"

// RUN: %clang -no-canonical-prefixes -target x86_64-pc-freebsd8 -static %s \
// RUN:   --sysroot=%S/Inputs/multiarch_freebsd64_tree -### 2>&1 \
// RUN:   | FileCheck --check-prefix=CHECK-STATIC %s
// CHECK-STATIC: crt1.o
// CHECK-STATIC: crtbeginT.o

// RUN: %clang -no-canonical-prefixes -target x86_64-pc-freebsd8 -shared %s \
// RUN:   --sysroot=%S/Inputs/multiarch_freebsd64_tree -### 2>&1 \
// RUN:   | FileCheck --check-prefix=CHECK-SHARED %s
// CHECK-SHARED: crti.o
// CHECK-SHARED: crtbeginS.o

// RUN: %clang -no-canonical-prefixes -target x86_64-pc-freebsd8 -pie %s \
// RUN:   --sysroot=%S/Inputs/multiarch_freebsd64_tree -### 2>&1 \
// RUN:   | FileCheck --check-prefix=CHECK-PIE %s
// CHECK-PIE: pie
// CHECK-PIE: Scrt1.o
// CHECK-PIE: crtbeginS.o

// RUN: %clang -no-canonical-prefixes -target x86_64-pc-freebsd8 %s \
// RUN:   --sysroot=%S/Inputs/multiarch_freebsd64_tree -### 2>&1 \
// RUN:   | FileCheck --check-prefix=CHECK-NORMAL %s
// CHECK-NORMAL: crt1.o
// CHECK-NORMAL: crtbegin.o

// RUN: %clang %s -### -o %t.o -target arm-unknown-freebsd10.0 2>&1 \
// RUN:   | FileCheck --check-prefix=CHECK-ARM %s
// CHECK-ARM: clang{{.*}}" "-cc1"{{.*}}" "-fsjlj-exceptions"
// CHECK-ARM: as{{.*}}" "-mfpu=softvfp"{{.*}}"-matpcs"

// RUN: %clang %s -### -o %t.o -target arm-gnueabi-freebsd10.0 2>&1 \
// RUN:   | FileCheck --check-prefix=CHECK-ARM-EABI %s
// CHECK-ARM-EABI-NOT: clang{{.*}}" "-cc1"{{.*}}" "-fsjlj-exceptions"
// CHECK-ARM-EABI: as{{.*}}" "-mfpu=softvfp"
// CHECK-ARM-EABI-NOT: as{{.*}}" "-matpcs"
