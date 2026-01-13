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


eor r24, r27; not x
and r24, r21; w
push r18
eor r18, r27
and r24, r18; not z
pop r18; end of w!x!z

push r28
mov r28, r20; x
push r21
eor r21, r27
and r28, r21; not w
pop r21; end of !wx
or r24, r28
pop r28; b = w!x!z + !wx


mov r23, r19; y
push r20
eor r20, r27
and r23, r20; not x
pop r20; end of y!x

push r28
mov r28, r19; y
push r21
eor r21, r27
and r28, r21; !w
pop r21
or r23, r28
pop r28; c = y!x + y!w

push r28
mov r28,r21; w
and r28, r20; wx
push r19
eor r19, r27
and r28, r19; wx!y
pop r19
or r23, r28
pop r28; c = y!x + y!w + wx!y

push r21
eor r21, r27
and r22, r21; z!w
pop r21

push r28
mov r28, r19; y
and r28, r20; x
and r28, r21; w 
or r22, r28 d = wxy + z!w
pop r28



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
