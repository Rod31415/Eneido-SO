#include "headers/idt.h"

extern "C" void _isr0();
extern "C" void _isr1();
extern "C" void _isr2();
extern "C" void _isr3();
extern "C" void _isr4();
extern "C" void _isr5();
extern "C" void _isr6();
extern "C" void _isr7();
extern "C" void _isr8();
extern "C" void _isr9();
extern "C" void _isr10();
extern "C" void _isr11();
extern "C" void _isr12();
extern "C" void _isr13();
extern "C" void _isr14();
extern "C" void _isr15();
extern "C" void _isr16();
extern "C" void _isr17();
extern "C" void _isr18();
extern "C" void _isr19();
extern "C" void _isr20();
extern "C" void _isr21();
extern "C" void _isr22();
extern "C" void _isr23();
extern "C" void _isr24();
extern "C" void _isr25();
extern "C" void _isr26();
extern "C" void _isr27();
extern "C" void _isr28();
extern "C" void _isr29();
extern "C" void _isr30();
extern "C" void _isr31();

extern "C" void _irq0();
extern "C" void _irq1();
extern "C" void _irq2();
extern "C" void _irq3();
extern "C" void _irq4();
extern "C" void _irq5();
extern "C" void _irq6();
extern "C" void _irq7();
extern "C" void _irq8();
extern "C" void _irq9();
extern "C" void _irq10();
extern "C" void _irq11();
extern "C" void _irq12();
extern "C" void _irq13();
extern "C" void _irq14();
extern "C" void _irq15();

struct regs
{
	unsigned int gs, fs, es, ds;                         /* pushed the segs last */
	unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax; /* pushed by 'pusha' */
	unsigned int int_no, err_code;                       /* our 'push byte #' and ecodes do this */
	unsigned int eip, cs, eflags, useresp, ss;           /* pushed by the processor automatically */
};

struct idt_entry
{
	uint16 isr_low;   // offset bits 0..15
	uint16 selector;  // a code segment selector in GDT or LDT
	uint8 zero;       // unused, set to 0
	uint8 attributes; // gate type, dpl, and p fields
	uint16 isr_high;  // offset bits 16..31
} __attribute__((packed));

struct idt_ptr
{
	uint16 limit;
	uint32 base;
} __attribute__((packed));

struct idt_entry idt[256] = {0};
struct idt_ptr _idtptr;

void idt_set_entry(uint8 vector, unsigned long isr, uint8 dpl)
{

	idt_entry *entry = &idt[vector];
	entry->isr_low = (uint32)isr & 0xFFFF;
	entry->isr_high = (uint32)isr >> 16;
	entry->selector = 0x8;
	entry->attributes = dpl;
	entry->zero = 0;
}

void isrs_install();
void irq_install_handler(int irq, void (*handler)(struct regs *r));
void irq_uninstall_handler(int irq);

extern "C" void idt_flush(uint32);

void idt_install()
{

	_idtptr.limit = (sizeof(struct idt_entry) * 256) - 1;
	_idtptr.base = (uint32)&idt;

	memset((uint32)&idt, 0, sizeof(struct idt_entry) * 256);

	outport(0x20, 0x11);
	outport(0xA0, 0x11);
	outport(0x21, 0x20);
	outport(0xA1, 0x28);
	outport(0x21, 0x04);
	outport(0xA1, 0x02);
	outport(0x21, 0x01);
	outport(0xA1, 0x01);
	outport(0x21, 0x00);
	outport(0xA1, 0x00);

	uint8 mask = inport(0xA1);
	mask = ~(1 << 3);
	outport(0xA1, mask);

	// uint8 mask=inport(0x21);
	// mask=~(1<<2);
	// outport(0x21,mask);

	isrs_install();

	irq_install_handler(11, rtl8139_irq_handler);
	irq_install_handler(1, keyboard_irq_handler);
	idt_flush((uint32)&_idtptr);
}

void isrs_install()
{
	idt_set_entry(0, (unsigned)_isr0, 0x8E);
	idt_set_entry(1, (unsigned)_isr1, 0x8E);
	idt_set_entry(2, (unsigned)_isr2, 0x8E);
	idt_set_entry(3, (unsigned)_isr3, 0x8E);
	idt_set_entry(4, (unsigned)_isr4, 0x8E);
	idt_set_entry(5, (unsigned)_isr5, 0x8E);
	idt_set_entry(6, (unsigned)_isr6, 0x8E);
	idt_set_entry(7, (unsigned)_isr7, 0x8E);
	idt_set_entry(8, (unsigned)_isr8, 0x8E);
	idt_set_entry(9, (unsigned)_isr9, 0x8E);
	idt_set_entry(10, (unsigned)_isr10, 0x8E);
	idt_set_entry(11, (unsigned)_isr11, 0x8E);
	idt_set_entry(12, (unsigned)_isr12, 0x8E);
	idt_set_entry(13, (unsigned)_isr13, 0x8E);
	idt_set_entry(14, (unsigned)_isr14, 0x8E);
	idt_set_entry(15, (unsigned)_isr15, 0x8E);
	idt_set_entry(16, (unsigned)_isr16, 0x8E);
	idt_set_entry(17, (unsigned)_isr17, 0x8E);
	idt_set_entry(18, (unsigned)_isr18, 0x8E);
	idt_set_entry(19, (unsigned)_isr19, 0x8E);
	idt_set_entry(20, (unsigned)_isr20, 0x8E);
	idt_set_entry(21, (unsigned)_isr21, 0x8E);
	idt_set_entry(22, (unsigned)_isr22, 0x8E);
	idt_set_entry(23, (unsigned)_isr23, 0x8E);
	idt_set_entry(24, (unsigned)_isr24, 0x8E);
	idt_set_entry(25, (unsigned)_isr25, 0x8E);
	idt_set_entry(26, (unsigned)_isr26, 0x8E);
	idt_set_entry(27, (unsigned)_isr27, 0x8E);
	idt_set_entry(28, (unsigned)_isr28, 0x8E);
	idt_set_entry(29, (unsigned)_isr29, 0x8E);
	idt_set_entry(30, (unsigned)_isr30, 0x8E);
	idt_set_entry(31, (unsigned)_isr31, 0x8E);

	idt_set_entry(32, (unsigned)_irq0, 0x8E);
	idt_set_entry(33, (unsigned)_irq1, 0x8E);
	idt_set_entry(34, (unsigned)_irq2, 0x8E);
	idt_set_entry(35, (unsigned)_irq3, 0x8E);
	idt_set_entry(36, (unsigned)_irq4, 0x8E);
	idt_set_entry(37, (unsigned)_irq5, 0x8E);
	idt_set_entry(38, (unsigned)_irq6, 0x8E);
	idt_set_entry(39, (unsigned)_irq7, 0x8E);
	idt_set_entry(40, (unsigned)_irq8, 0x8E);
	idt_set_entry(41, (unsigned)_irq9, 0x8E);
	idt_set_entry(42, (unsigned)_irq10, 0x8E);
	idt_set_entry(43, (unsigned)_irq11, 0x8E);
	idt_set_entry(44, (unsigned)_irq12, 0x8E);
	idt_set_entry(45, (unsigned)_irq13, 0x8E);
	idt_set_entry(46, (unsigned)_irq14, 0x8E);
	idt_set_entry(47, (unsigned)_irq15, 0x8E);
}

char *exception_messages[] =
		{
	"Division por cero",
	"Debug",
	"Non Maskable Interrupt",
	"Breakpoint",
	"Into Detected Overflow",
	"Out of Bounds",
	"Invalid Opcode",
	"No Coprocessor",

	"Double Fault",
	"Coprocessor Segment Overrun",
	"Bad TSS",
	"Segment Not Present",
	"Stack Fault",
	"General Protection Fault",
	"Page Fault",
	"Unknown Interrupt",

	"Coprocessor Fault",
	"Alignment Check",
	"Machine Check",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",

	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved"};

extern "C" void fault_handler(struct regs *r)
{
	if (r->int_no < 32)
	{

		printf(exception_messages[r->int_no]);
		printf(" Excepcion. El sistema se paro/n");
		printf(" Porfavor apague el equipo/n");
		refresh();
		for (;;)
			;
	}
}

void (*irq_routines[16])(struct regs *r) = {
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0};

void irq_install_handler(int irq, void (*handler)(struct regs *r))
{

	irq_routines[irq] = handler;
}

void irq_uninstall_handler(int irq)
{

	irq_routines[irq] = 0;
}

extern "C" void irq_handler(struct regs *r)
{
	void (*handler)(struct regs *r);
	handler = irq_routines[r->int_no - 32];

	 if(r->int_no==34)
	 printf(" Excepcion n%d/n",r->int_no-32);
	   refresh();
	if (handler)
	{
		handler(r);
	}

	if (r->int_no >= 40)
		outport(0xA0, 0x20);
	outport(0x20, 0x20);
}
