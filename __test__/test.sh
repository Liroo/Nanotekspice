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
echo "display"| ../nanotekspice resources/4017.nts clock=1 reset=0 disable=0 > .tmp4017
echo "simulate\ndisplay"| ../nanotekspice resources/4017.nts clock=0 reset=0 disable=0 >> .tmp4017
echo "simulate\nsimulate\nsimulate\nsimulate\nsimulate\nsimulate\nsimulate\nsimulate\nsimulate\ndisplay"| ../nanotekspice resources/4017.nts clock=0 reset=0 disable=0 >> .tmp4017
echo "simulate\nreset=1\nsimulate\nreset=0\nsimulate\ndisplay"| ../nanotekspice resources/4017.nts clock=0 reset=0 disable=1 >> .tmp4017

#
#      TESTING COMPONENT 4030
#
echo "display" | ../nanotekspice resources/4030.nts a=0 b=0 a2=0 b2=1 a3=1 b3=1 a4=1 b4=0  > .tmp4030

#
#      TESTING COMPONENT 4040
#
echo "display"| ../nanotekspice resources/4040.nts clock=0 reset=0 > .tmp4040;
echo "simulate\ndisplay"| ../nanotekspice resources/4040.nts clock=1 reset=0 >> .tmp4040;
echo "simulate\nsimulate\nsimulate\nsimulate\nsimulate\nsimulate\nsimulate\nsimulate\nsimulate\ndisplay"| ../nanotekspice resources/4040.nts clock=1 reset=0 >> .tmp4040;
echo "simulate\nreset=1\nsimulate\nreset=0\nsimulate\ndisplay"| ../nanotekspice resources/4040.nts clock=1 reset=0 >> .tmp4040;

#
#      TESTING COMPONENT 4069
#
echo "display" | ../nanotekspice resources/4069.nts i0=0 i1=1 > .tmp4069

#
#      TESTING COMPONENT 4071
#
echo "display" | ../nanotekspice resources/4071.nts a=0 b=0 a2=0 b2=1 a3=1 b3=1 a4=1 b4=0  > .tmp4071

#
#      TESTING COMPONENT 4081
#
echo "display" | ../nanotekspice resources/4081.nts a=0 b=0 a2=0 b2=1 a3=1 b3=1 a4=1 b4=0  > .tmp4081

#
#      TESTING COMPONENT 4094
#
echo "display" | ../nanotekspice resources/4094.nts strobe=0 data=0 cp=0 enable=0 > .tmp4094
echo "display" | ../nanotekspice resources/4094.nts strobe=0 data=0 cp=0 enable=1 >> .tmp4094
echo "display" | ../nanotekspice resources/4094.nts strobe=0 data=0 cp=1 enable=0 >> .tmp4094
echo "display" | ../nanotekspice resources/4094.nts strobe=0 data=1 cp=0 enable=0 >> .tmp4094
echo "display" | ../nanotekspice resources/4094.nts strobe=1 data=0 cp=0 enable=0 >> .tmp4094
echo "display" | ../nanotekspice resources/4094.nts strobe=1 data=1 cp=0 enable=0 >> .tmp4094
echo "display" | ../nanotekspice resources/4094.nts strobe=1 data=0 cp=1 enable=0 >> .tmp4094
echo "display" | ../nanotekspice resources/4094.nts strobe=1 data=0 cp=0 enable=1 >> .tmp4094
echo "display" | ../nanotekspice resources/4094.nts strobe=0 data=1 cp=1 enable=0 >> .tmp4094
echo "display" | ../nanotekspice resources/4094.nts strobe=0 data=1 cp=0 enable=1 >> .tmp4094
echo "display" | ../nanotekspice resources/4094.nts strobe=0 data=0 cp=1 enable=1 >> .tmp4094
echo "display" | ../nanotekspice resources/4094.nts strobe=1 data=1 cp=1 enable=0 >> .tmp4094
echo "display" | ../nanotekspice resources/4094.nts strobe=1 data=1 cp=0 enable=1 >> .tmp4094
echo "display" | ../nanotekspice resources/4094.nts strobe=1 data=0 cp=1 enable=1 >> .tmp4094
echo "display" | ../nanotekspice resources/4094.nts strobe=0 data=1 cp=1 enable=1 >> .tmp4094
echo "display" | ../nanotekspice resources/4094.nts strobe=1 data=1 cp=1 enable=1 >> .tmp4094

#
#      TESTING COMPONENT 4514
#
echo "display" | ../nanotekspice resources/4514.nts a0=0 a1=0 a2=0 a3=0 inhibit=1 strobe=0 > .tmp4594;
echo "display" | ../nanotekspice resources/4514.nts a0=0 a1=0 a2=0 a3=0 inhibit=0 strobe=0 >> .tmp4594;
echo "display" | ../nanotekspice resources/4514.nts a0=1 a1=0 a2=0 a3=0 inhibit=0 strobe=0 >> .tmp4594;
echo "display" | ../nanotekspice resources/4514.nts a0=0 a1=1 a2=0 a3=0 inhibit=0 strobe=0 >> .tmp4594;
echo "display" | ../nanotekspice resources/4514.nts a0=0 a1=0 a2=1 a3=0 inhibit=0 strobe=0 >> .tmp4594;
echo "display" | ../nanotekspice resources/4514.nts a0=0 a1=0 a2=0 a3=1 inhibit=0 strobe=0 >> .tmp4594;
echo "display" | ../nanotekspice resources/4514.nts a0=0 a1=0 a2=0 a3=0 inhibit=0 strobe=1 >> .tmp4594;
echo "display" | ../nanotekspice resources/4514.nts a0=1 a1=0 a2=1 a3=0 strobe=0 inhibit=0 >> .tmp4594;
echo "display" | ../nanotekspice resources/4514.nts a0=1 a1=0 a2=0 a3=1 strobe=0 inhibit=0 >> .tmp4594;
echo "display" | ../nanotekspice resources/4514.nts a0=1 a1=0 a2=0 a3=0 strobe=1 inhibit=0 >> .tmp4594;
echo "display" | ../nanotekspice resources/4514.nts a0=0 a1=1 a2=1 a3=0 strobe=0 inhibit=0 >> .tmp4594;
echo "display" | ../nanotekspice resources/4514.nts a0=0 a1=1 a2=0 a3=1 strobe=0 inhibit=0 >> .tmp4594;
echo "display" | ../nanotekspice resources/4514.nts a0=0 a1=0 a2=1 a3=1 strobe=0 inhibit=0 >> .tmp4594;
echo "display" | ../nanotekspice resources/4514.nts a0=1 a1=1 a2=1 a3=0 strobe=0 inhibit=0 >> .tmp4594;
echo "display" | ../nanotekspice resources/4514.nts a0=1 a1=1 a2=0 a3=1 strobe=0 inhibit=0 >> .tmp4594;
echo "display" | ../nanotekspice resources/4514.nts a0=0 a1=1 a2=1 a3=1 strobe=0 inhibit=0 >> .tmp4594;
echo "display" | ../nanotekspice resources/4514.nts a0=1 a1=1 a2=1 a3=1 strobe=0 inhibit=0 >> .tmp4594;

diff .tmp4001 answers/4001 > /dev/null
if [ $? -eq 0 ]
then
    echo "Tests 4001 FOUR NOR GATES \t\t\tPASSED"
else
    echo "Tests 4001 FOUR NOR GATES \t\t\tFAILED"
fi

diff .tmp4008 answers/4008 > /dev/null
if [ $? -eq 0 ]
then
    echo "Tests 4008 4bits adder \t\t\t\tPASSED"
else
    echo "Tests 4008 4bits adder \t\t\t\tFAILED"
fi

diff .tmp4011 answers/4011 > /dev/null
if [ $? -eq 0 ]
then
    echo "Tests 4011 FOUR NAND GATES \t\t\tPASSED"
else
    echo "Tests 4011 FOUR NAND GATES \t\t\tFAILED"
fi

diff .tmp4013 answers/4013 > /dev/null
if [ $? -eq 0 ]
then
    echo "Tests 4013 DUAL FLIP FLOP \t\t\tPASSED"
else
    echo "Tests 4013 DUAL FLIP FLOP \t\t\tFAILED"
fi

diff .tmp4017 answers/4017 > /dev/null
if [ $? -eq 0 ]
then
    echo "Tests 4017 10bits Johson Decade \t\tPASSED"
else
    echo "Tests 4017 10bits Johson Decade \t\tFAILED"
fi

diff .tmp4030 answers/4030 > /dev/null
if [ $? -eq 0 ]
then
    echo "Tests 4030 FOUR XOR GATES \t\t\tPASSED"
else
    echo "Tests 4030 FOUR XOR GATES \t\t\tFAILED"
fi

diff .tmp4040 answers/4040 > /dev/null
if [ $? -eq 0 ]
then
    echo "Tests 4040 12bits counter \t\t\tPASSED"
else
    echo "Tests 4040 12bits counter \t\t\tFAILED"
fi

diff .tmp4069 answers/4069 > /dev/null
if [ $? -eq 0 ]
then
    echo "Tests 4069 SIX INVERTER GATES \t\t\tPASSED"
else
    echo "Tests 4069 SIX INVERTER GATES \t\t\tFAILED"
fi

diff .tmp4071 answers/4071 > /dev/null
if [ $? -eq 0 ]
then
    echo "Tests 4071 FOUR OR GATES \t\t\tPASSED"
else
    echo "Tests 4071 FOUR OR GATES \t\t\tFAILED"
fi

diff .tmp4081 answers/4081 > /dev/null
if [ $? -eq 0 ]
then
    echo "Tests 4081 FOUR AND GATES \t\t\tPASSED"
else
    echo "Tests 4081 FOUR AND GATES \t\t\tFAILED"
fi

diff .tmp4094 answers/4094 > /dev/null
if [ $? -eq 0 ]
then
    echo "Tests 4094 8bits shift register \t\tPASSED"
else
    echo "Tests 4094 8bits shift register \t\tFAILED"
fi

diff .tmp4594 answers/4594 > /dev/null
if [ $? -eq 0 ]
then
    echo "Tests 4514 4bits decoder \t\t\tPASSED"
else
    echo "Tests 4514 4bits decoder \t\t\tFAILED"
fi

rm -f .tmp*
