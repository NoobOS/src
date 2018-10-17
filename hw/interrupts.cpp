
#include "../../include/common/types.h"
#include "../../include/hw/interrupts.h"
#include "../../include/hw/port.h"

void printf(char* str);

InterruptManager::GateDescriptor InterruptManager::InterruptDescriptorTable[256];
    
void InterruptManager::SetInterruptDescriptorTableEntry(
        uint8_t interruptnumber,
        uint16_t codeSegmentSelectorOffset,
        void (*handler)(),
        uint8_t DescriptorPrivilegeLevel, 
        uint8_t DescriptorType){
    
        const uint8_t IDT_DESC_PRESENT = 0x80;
            
        InterruptDescriptorTable[interruptnumber].handlerAddressLowBits = ((uint32_t) handler) & 0xFFFF;
        InterruptDescriptorTable[interruptnumber].handlerAddressHighBits = ((uint32_t) handler >> 16) & 0xFFFF;
        InterruptDescriptorTable[interruptnumber].gdt_codeSegmentSelector = codeSegmentSelectorOffset;
        InterruptDescriptorTable[interruptnumber].access = IDT_DESC_PRESENT | DescriptorType| ((DescriptorPrivilegeLevel & 3) << 5);
        InterruptDescriptorTable[interruptnumber].reserved = 0;        
    
        }

    
InterruptManager::InterruptManager(GlobalDescriptorTable* gdt){
    
    uint16_t CodeSegment = gdt ->CodeSegmentSelector();
    const uint8_t IDT_INTERRUPT_GATE = 0xE;
    
    for (uint16_t i=0; i < 256; i++){
        SetInterruptDescriptorTableEntry(i , CodeSegment, &IgnoreInterruptRequest, 0, IDT_INTERRUPT_GATE);
    }
    
    SetInterruptDescriptorTableEntry(0x20, CodeSegment, &HandleInterruptRequest0x00, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x21, CodeSegment, &HandleInterruptRequest0x01, 0, IDT_INTERRUPT_GATE);
    
    InterruptDescriptorTablePointer idt;
    idt.size = 256*sizeof(GateDescriptor) - 1;
    idt.base = (uint32_t)InterruptDescriptorTable;
    asm volatile("lidt %0" : : "m" (idt));
    
}
InterruptManager::~InterruptManager(){}

void InterruptManager::Activate(){
    
    asm("sti");
}

uint32_t InterruptManager::HandleInterrupt(uint8_t interruptnumber,uint32_t esp){
    
    printf("Interrupt");
    return esp;
}