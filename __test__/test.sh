#!/bin/sh

#
# TESTING COMPONENT 4001
#
echo "display" | ../nanotekspice resources/4001.nts a=0 b=0 a2=0 b2=1 a3=1 b3=1 a4=1 b4=0  > .tmp4001

#
# TESTING COMPONENT 4008
#
echo "display" | ../nanotekspice resources/4008.nts a=0 b=0 a2=1 b2=1 a3=1 b3=1 a4=1 b4=1 cin=0 > .tmp4008
echo "display" | ../nanotekspice resources/4008.nts a=0 b=0 a2=1 b2=1 a3=1 b3=1 a4=1 b4=1 cin=1 >> .tmp4008
echo "display" | ../nanotekspice resources/4008.nts a=0 b=1 a2=1 b2=1 a3=1 b3=1 a4=1 b4=1 cin=0 >> .tmp4008
echo "display" | ../nanotekspice resources/4008.nts a=1 b=0 a2=1 b2=1 a3=1 b3=1 a4=1 b4=1 cin=0 >> .tmp4008
echo "display" | ../nanotekspice resources/4008.nts a=0 b=1 a2=1 b2=1 a3=1 b3=1 a4=1 b4=1 cin=1 >> .tmp4008
echo "display" | ../nanotekspice resources/4008.nts a=1 b=0 a2=1 b2=1 a3=1 b3=1 a4=1 b4=1 cin=1 >> .tmp4008
echo "display" | ../nanotekspice resources/4008.nts a=1 b=1 a2=1 b2=1 a3=1 b3=1 a4=1 b4=1 cin=0 >> .tmp4008
echo "display" | ../nanotekspice resources/4008.nts a=1 b=1 a2=1 b2=1 a3=1 b3=1 a4=1 b4=1 cin=1 >> .tmp4008

#
# TESTING COMPONENT 4011
#

echo "display" | ../nanotekspice resources/4011.nts a=0 b=0 a2=0 b2=1 a3=1 b3=1 a4=1 b4=0  > .tmp4011

#
#      TESTING COMPONENT 4013
#

# tests check rising clock when clock initialized at 0
echo "display"| ../nanotekspice resources/4013.nts clock=0 data=0 set=0 reset=1 > .tmp4013
echo "display"| ../nanotekspice resources/4013.nts clock=0 data=0 set=1 reset=0 >> .tmp4013
echo "display"| ../nanotekspice resources/4013.nts clock=0 data=1 set=0 reset=0 >> .tmp4013
echo "display"| ../nanotekspice resources/4013.nts clock=0 data=1 set=1 reset=0 >> .tmp4013
echo "display"| ../nanotekspice resources/4013.nts clock=0 data=1 set=0 reset=1 >> .tmp4013
echo "display"| ../nanotekspice resources/4013.nts clock=0 data=1 set=1 reset=1 >> .tmp4013
echo "display"| ../nanotekspice resources/4013.nts clock=0 data=1 set=1 reset=1 >> .tmp4013
# tests check rising clock when clock initialized at 1
echo "display"| ../nanotekspice resources/4013.nts clock=1 data=0 set=0 reset=0 >> .tmp4013
echo "display"| ../nanotekspice resources/4013.nts clock=1 data=0 set=0 reset=1 >> .tmp4013
echo "display"| ../nanotekspice resources/4013.nts clock=1 data=0 set=1 reset=0 >> .tmp4013
echo "display"| ../nanotekspice resources/4013.nts clock=1 data=1 set=0 reset=0 >> .tmp4013
echo "display"| ../nanotekspice resources/4013.nts clock=1 data=1 set=1 reset=0 >> .tmp4013
echo "display"| ../nanotekspice resources/4013.nts clock=1 data=1 set=0 reset=1 >> .tmp4013
echo "display"| ../nanotekspice resources/4013.nts clock=1 data=1 set=1 reset=1 >> .tmp4013
echo "display"| ../nanotekspice resources/4013.nts clock=1 data=1 set=1 reset=1 >> .tmp4013
# tests when clock rising
echo "simulate\ndisplay"| ../nanotekspice resources/4013.nts clock=0 data=0 set=0 reset=0 >> .tmp4013
echo "simulate\ndisplay"| ../nanotekspice resources/4013.nts clock=0 data=0 set=0 reset=1 >> .tmp4013
echo "simulate\ndisplay"| ../nanotekspice resources/4013.nts clock=0 data=0 set=1 reset=0 >> .tmp4013
echo "simulate\ndisplay"| ../nanotekspice resources/4013.nts clock=0 data=1 set=0 reset=0 >> .tmp4013
echo "simulate\ndisplay"| ../nanotekspice resources/4013.nts clock=0 data=1 set=1 reset=0 >> .tmp4013
echo "simulate\ndisplay"| ../nanotekspice resources/4013.nts clock=0 data=1 set=0 reset=1 >> .tmp4013
echo "simulate\ndisplay"| ../nanotekspice resources/4013.nts clock=0 data=1 set=1 reset=1 >> .tmp4013
echo "simulate\ndisplay"| ../nanotekspice resources/4013.nts clock=0 data=1 set=1 reset=1 >> .tmp4013
# tests when clock !rising
echo "simulate\ndisplay"| ../nanotekspice resources/4013.nts clock=1 data=0 set=0 reset=0 >> .tmp4013
echo "simulate\ndisplay"| ../nanotekspice resources/4013.nts clock=1 data=0 set=0 reset=1 >> .tmp4013
echo "simulate\ndisplay"| ../nanotekspice resources/4013.nts clock=1 data=0 set=1 reset=0 >> .tmp4013
echo "simulate\ndisplay"| ../nanotekspice resources/4013.nts clock=1 data=1 set=0 reset=0 >> .tmp4013
echo "simulate\ndisplay"| ../nanotekspice resources/4013.nts clock=1 data=1 set=1 reset=0 >> .tmp4013
echo "simulate\ndisplay"| ../nanotekspice resources/4013.nts clock=1 data=1 set=0 reset=1 >> .tmp4013
echo "simulate\ndisplay"| ../nanotekspice resources/4013.nts clock=1 data=1 set=1 reset=1 >> .tmp4013
echo "simulate\ndisplay"| ../nanotekspice resources/4013.nts clock=1 data=1 set=1 reset=1 >> .tmp4013
echo "simulate\ndisplay"| ../nanotekspice resources/4013.nts clock=0 data=0 set=0 reset=0 >> .tmp4013

#
#      TESTING COMPONENT 4017
#
echo "simulate\ndisplay"| ../nanotekspice resources/4013.nts clock=0 data=0 set=0 reset=0 >> .tmp4013


#echo "display" | ../nanotekspice resources/not.nts i0=0 i1=1 > .tmp4069
#echo "display" | ../nanotekspice resources/or.nts a=0 b=0 a2=0 b2=1 a3=1 b3=1 a4=1 b4=0  > .tmp4071
#echo "display" | ../nanotekspice resources/and.nts a=0 b=0 a2=0 b2=1 a3=1 b3=1 a4=1 b4=0  > .tmp4081
#echo "display" | ../nanotekspice resources/xor.nts a=0 b=0 a2=0 b2=1 a3=1 b3=1 a4=1 b4=0  > .tmp4030


diff .tmp4001 answers/4001 > /dev/null
if [ $? -eq 0 ]
then
    echo "Tests 4001 FOUR NOR GATES passed"
else
    echo "Tests 4001 FOUR NOR GATES failed"
fi

diff .tmp4008 answers/4008 > /dev/null
if [ $? -eq 0 ]
then
    echo "Tests 4008 4bits adder passed"
else
    echo "Tests 4008 4bits adder failed"
fi

diff .tmp4011 answers/4011 > /dev/null
if [ $? -eq 0 ]
then
    echo "Tests 4011 FOUR NAND GATES passed"
else
    echo "Tests 4011 FOUR NAND GATES failed"
fi

diff .tmp4013 answers/4013 > /dev/null
if [ $? -eq 0 ]
then
    echo "Tests 4013 DUAL FLIP FLOP passed"
else
    echo "Tests 4013 DUAL FLIP FLOP failed"
fi

diff .tmp4017 answers/4017 > /dev/null
if [ $? -eq 0 ]
then
    echo "Tests 4017 10bits Johson Decade passed"
else
    echo "Tests 4017 10bits Johson Decade failed"
fi



diff .tmp4069 res/4069 > /dev/null
if [ $? -eq 0 ]
then
    echo "Tests 4069 passed"
else
    echo "Tests 4069 failed"
fi

diff .tmp4071 res/4071 > /dev/null
if [ $? -eq 0 ]
then
    echo "Tests 4071 passed"
else
    echo "Tests 4071 failed"
fi

diff .tmp4081 res/4081 > /dev/null
if [ $? -eq 0 ]
then
    echo "Tests 4081 passed"
else
    echo "Tests 4081 failed"
fi

diff .tmp4030 res/4030 > /dev/null
if [ $? -eq 0 ]
then
    echo "Tests 4030 passed"
else
    echo "Tests 4030 failed"
fi

# tests witch clock 

rm -f .tmp*
