
/**
 * Copyright (C) 2019 bolthur project.
 *
 * This file is part of bolthur/serial-loader.
 *
 * bolthur/serial-loader is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * bolthur/serial-loader is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with bolthur/serial-loader.  If not, see <http://www.gnu.org/licenses/>.
 */

.arch_extension virt

.section .text.boot

.global boot_start
boot_start:
  // setup temporary stack
  ldr r3, =boot_start
  mov sp, r3

  // check cpu id and skip if not 0
  cpsid if // disable irq & fiq
  // r3 = multiprocessor affinity register ( mpidr )
  mrc p15, 0, r3, c0, c0, 5
  ands r3, #3 // r3 = cpu id ( bits 0 to 1 )
  // if ( cpu id not equal to 0 ) branch to infinite loop ( core id 1 to 3 )
  bne 1f
  // if equal to 0 branch to normal startup
  beq 2f
  1:
    wfe
    b 1
  2:

  // check for hypervisor mode and switch back to supervisor mode
  mrs r3, cpsr
  eor r3, r3, #0x1A
  tst r3, #0x1F
  // clear mode bits
  bic r3, r3, #0x1F
  // mask irq and fiq bits and set svc mode
  orr r3, r3, #( 0x13 | 0x40 | 0x80 )
  // branch if not HYP mode
  bne 3f
  // mask abort bit
  orr r3, r3, #0x100
  adr lr, 4f
  msr spsr_cxsf, r3
  msr ELR_hyp, lr
  eret
  3: msr cpsr_c, r3
  4:

  // setup temporary stack again necessary due to mode switch
  ldr r4, =boot_start
  mov sp, r4

  // Set SMP bit within extended control register according to documentation
  mrrc p15, 1, r3, r1, c15
  orr r3, r3, #( 1 << 6 )
  mcrr p15, 1, r3, r1, c15

  // read system control register
  mrc p15, 0, r3, c1, c0, 0
  // disable data cache
  bic r3, r3, #( 1 << 2 )
  // disable instruction cache
  bic r3, r3, #( 1 << 12 )
  // push back changes
  mcr p15, 0, r3, c1, c0, 0

  // FIXME: Clear bss section
  // FIXME: Save boot parameter somewhere
  // FIXME: Relocate loader code from fixed load address to different address

  // switch to c main
  ldr r3, =loader_main
  blx r3

halt:
  wfe // equivalent of x86 HLT instruction
  b halt
