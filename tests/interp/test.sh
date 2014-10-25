#! env sh

for i in ../emu/*;
do
    ../../chip8-emulator -d $i > $i.asm;
    ../../chip8-emulator -i $i.asm;
    diff $i $i.asm.out;
    echo $?;
    rm -f ../emu/*.asm;
done
