#include "headers/rtl8139.h"

struct RTL8139
{
	PCIDescriptor *descriptor;
	PCIGeneralDevice *device;
	uint32 iobase;
	uint8 MAC[6];
};
uint32 generalIobase;
enum RTL8139Registers
{
	RTL8139_REG_MAC0_5 = 0x00,
	RTL8139_REG_MAC5_6 = 0x04,
	RTL8139_REG_MAR0_7 = 0x08,
	RTL8139_REG_RBSTART = 0x30,
	RTL8139_REG_CMD = 0x37,
	RTL8139_REG_IMR = 0x3C,
	RTL8139_REG_ISR = 0x3E,

	RTL8139_REG_POWERUP = 0x52
};

RTL8139 *actualRTL;

PCIDescriptor *d;

uint8 rx_buffer[8192 + 16];
uint8 tx_buffer[4][1536];

int32 current_tx = 0;

uint32 txAddrRegs[4] = {0x20, 0x24, 0x28, 0x2c};
uint32 txStatRegs[4] = {0x10, 0x14, 0x18, 0x1c};

void printMAC()
{
	printf("MAC :");
	printf("%x", actualRTL->MAC[0]);
	for (uint32 a = 1; a < 6; a++)
	{
		printf(":%x", actualRTL->MAC[a]);
	}
}

bool isRtl8139(PCIDescriptor *desc)
{
	if (desc->vendor_id == 0x10EC && desc->device_id == 0x8139)
		return true;
	return false;
}

void Rtl8139SendPacket(uint8 *MACDest, uint8 *buff, uint32 length);
void Rtl8139RecvPacket();

void rtl8139_irq_handler(struct regs *r)
{
	actualRTL->iobase = generalIobase;

	uint16 status = inportw(actualRTL->iobase + 0x3E);
	outportw(actualRTL->iobase + 0x3E, 0x05);

	if (status & (1 << 2))
	{
		printf("Paquete Enviado");
	}
	if (status & (1 << 0))
	{
		// printf("Paquete Recibido/n");
		Rtl8139RecvPacket();
	}
}
static RTL8139 RTLd;
bool initRtl8139(PCIDescriptor *desc)
{
	if (!isRtl8139(desc))
	{
		printf("TODO MAL/n");
		return false;
	}

	PCIBusMastering(desc);

	PCIGeneralDevice *rtl;
	PCIGetGeneralDevice(desc, rtl);
	actualRTL = &RTLd;

	actualRTL->device = rtl;
	actualRTL->iobase = rtl->base_address[0] & ~0x3;

	// Prendiendo el rtl
	outport(actualRTL->iobase + 0x52, 0x0);

	// Software reset
	outport(actualRTL->iobase + 0x37, 0x10);
	while (inport(actualRTL->iobase + 0x37) & 0x10)
		;

	// Iniciar el buffer para recibir datos
	outportl(actualRTL->iobase + 0x30, (uint32)rx_buffer);

	// Set the Interrupt Mask Register (IMR) and Interrupt Service Register (ISR)

	outportw(actualRTL->iobase + 0x3c, 0x0005);

	// outportw(actualRTL->iobase+0x3e,0xFFFF);

	// Pa configurar el buffer
	outportl(actualRTL->iobase + 0x44, 0xf | (1 << 7) | (1 << 4));

	// Habilitar RX y TX
	outport(actualRTL->iobase + 0x37, 0x0C);

	uint32 m0 = inportl(actualRTL->iobase + RTL8139_REG_MAC0_5);
	uint32 m1 = inportw(actualRTL->iobase + RTL8139_REG_MAC5_6);

	actualRTL->MAC[0] = m0;
	actualRTL->MAC[1] = m0 >> 8;
	actualRTL->MAC[2] = m0 >> 16;
	actualRTL->MAC[3] = m0 >> 24;

	actualRTL->MAC[4] = m1;
	actualRTL->MAC[5] = m1 >> 8;

	printf("  |%d|  ", actualRTL->iobase);
	generalIobase = actualRTL->iobase;

	return true;
}

void Rtl8139EnableIRQs()
{
	actualRTL->iobase = generalIobase;

	outportw(actualRTL->iobase + 0x3c, 0x0000);

	uint16 pending = inportw(actualRTL->iobase + 0x3e);

	outportw(actualRTL->iobase + 0x3e, 0xFFFF);

	pending = inportw(actualRTL->iobase + 0x3e);

	outportw(actualRTL->iobase + 0x3c, 0x0005);
}

void Rtl8139SendPacket(uint8 *MACDest, uint8 *buff, uint32 length)
{
	actualRTL->iobase = generalIobase;
	printf("|%d| : %d bytes ", actualRTL->iobase, length);
	uint8 *newbuffer = tx_buffer[current_tx];
	for (uint32 i = 0; i < 6; i++)
	{
		newbuffer[i] = MACDest[i];
		newbuffer[i + 6] = actualRTL->MAC[i];
	}
	newbuffer[12] = 0x08;
	newbuffer[13] = 0x00;

	for (uint32 i = 0; i < length; i++)
	{
		newbuffer[i + 14] = buff[i];
	}
	uint32 total_len = length + 14;
	if (total_len < 60)
	{
		for (uint32 i = length + 14; i < 60; i++)
		{
			newbuffer[i] = 0;
		}
		total_len = 60;
	}

	outportl(actualRTL->iobase + txAddrRegs[current_tx], (uint32)newbuffer);
	outportl(actualRTL->iobase + txStatRegs[current_tx], total_len & 0x1FFF);

	printf(" %d ", current_tx);

	current_tx = (current_tx + 1) % 4;
}

uint32 current_rx = 0;

void Rtl8139RecvPacket()
{
	actualRTL->iobase = generalIobase;

	uint16 rx_status = *(uint16 *)(rx_buffer + current_rx);
	uint16 rx_size = *(uint16 *)(rx_buffer + current_rx + 2);

	uint8 *packet = rx_buffer + current_rx + 4;

	/*printf("Datos:/n");
	for(uint32 i=0;i<rx_size;i++){
		printf("%c",packet[i]);
		if((i+1)%32==0)printf("|/n");
	}*/

	current_rx = (uint32)(current_rx + rx_buffer + 4 + 3) & ~3;

	outportw(actualRTL->iobase + 0x38, current_rx - 16);
	outportw(actualRTL->iobase + 0x3E, (1 << 0));
}
