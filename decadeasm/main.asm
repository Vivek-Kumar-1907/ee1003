.include "m328Pdef.inc"

ldi r16, 0b00111100
out ddrd, r16
cbi ddrb, 0
cbi ddrb, 1
sbi ddrb, 5

push r16
ldi r16, 0b00000101
out TCCR0B, r16
pop r16

ldi r26, 0
ldi r27, 1

ldi r31, 5

start:
push r16
in r16, pinb
bst r16, 1
ldi r18, 0
bld r18, 0; z-msb

in r16, pinb
bst r16, 0
ldi r19, 0
bld r19, 0; y

in r16, pind
bst r16, 7
ldi r20, 0
bld r20, 0; x

in r16, pind
bst r16, 6
ldi r21, 0; 
bld r21, 0; w-lsb
pop r16

mov r22, r18
mov r23, r19
mov r24, r20
mov r25, r21

eor r25, r27; a = not w


mov r24, r20; b = x
and r24, r21; w
push r18
eor r18, r27
and r24, r18; not z
pop r18; end of wx!z

push r28
mov r28, r18; z
eor r28, r19
push r21
eor r21, r27
and r28, r21; not w
pop r21; end of (z xor y)!w
push r20
eor r20, r27
and r28, r20; not x
pop r20
or r24, r28
pop r28; b = wx!z + (z xor y)!w!x




mov r23, r18; z
push r21
eor r21, r27
and r23, r21; not w
pop r21; end of z!w

push r28
mov r28, r19; y
and r28, r21; w
or r23, r28
pop r28; c = yw+z!w

push r28
mov r28,r20; x
and r28, r19; yx
or r23, r28
pop r28; c=yw+z!w+yx

push r28
push r18
eor r18,r27
mov r28 ,r18
pop r18
push r19
eor r19,r27
and r28,r19
pop r19
push r20
eor r20,r27
and r28,r20
pop r20
push r21
eor r21,r27
and r28,r21
pop r21
mov r22, r28
pop r28;!Z!X!Y!W

push r28 
mov r28,r18;z
and r28,r21;zw
or r22,r28
pop r28;d=!z!x!y!w + zw



ldi r16, 5
mov r30, r22
rcall shift
mov r22, r30

ldi r16, 4
mov r30, r23
rcall shift
mov r23, r30

ldi r16, 3
mov r30, r24
rcall shift
mov r24, r30

ldi r16, 2
mov r30, r25
rcall shift
mov r25, r30

ldi r17, 0
add r17, r22
add r17, r23
add r17, r24
add r17, r25

out portd, r17

sbi portb, 5
push r16
ldi r16, 61
rcall delay; delay
pop r16
cbi portb, 5
push r16
ldi r16, 61
rcall delay; delay
pop r16
rjmp start

loop:
	rjmp loop

jumpback:
	rjmp start

load:
	ldi r16, 0
	ret

shift:
	lsl r30
	dec r16
	brne shift
	ret

delay:
	push r31
	in r31, TIFR0
	push r27
	ldi r27, 2
	and r31, r27
	pop r27
	pop r31
	breq delay
	push r27
	ldi r27, 2
	out TIFR0, r27
	pop r27
	dec r16
	brne delay
	ldi r16, 61
	ret
