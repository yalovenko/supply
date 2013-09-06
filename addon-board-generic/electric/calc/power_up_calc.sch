<Qucs Schematic 0.0.17>
<Properties>
  <View=0,-120,1033,949,0.909091,93,292>
  <Grid=10,10,1>
  <DataSet=power_up_calc.dat>
  <DataDisplay=power_up_calc.dpl>
  <OpenDisplay=1>
  <Script=power_up_calc.m>
  <RunScript=0>
  <showFrame=0>
  <FrameText0=Title>
  <FrameText1=Drawn By:>
  <FrameText2=Date:>
  <FrameText3=Revision:>
</Properties>
<Symbol>
</Symbol>
<Components>
  <GND * 1 540 500 0 0 0 0>
  <GND * 1 280 500 0 0 0 0>
  <GND * 1 920 500 0 0 0 0>
  <_MOSFET T1 1 540 290 -26 -68 1 1 "pfet" 0 "-1.0 V" 1 "2e-5" 1 "0.0" 0 "0.6 V" 0 "0.0" 1 "0.0 Ohm" 0 "0.0 Ohm" 0 "0.0 Ohm" 0 "1e-14 A" 0 "1.0" 0 "1 um" 0 "1 um" 0 "0.0" 0 "0.1 um" 0 "100 pF" 0 "100 pF" 0 "100 pF" 0 "100 pF" 0 "100 pF" 0 "0.8 V" 0 "0.5" 0 "0.5" 0 "0.0" 0 "0.33" 0 "0.0 ps" 0 "0.0" 0 "0.0" 0 "1" 0 "600.0" 0 "0.0" 0 "1" 0 "1" 0 "0.0" 0 "0.0" 0 "0.0" 0 "0.0" 0 "0.0 m" 0 "0.0 m" 0 "0.0" 0 "1.0" 0 "1.0" 0 "26.85" 0 "26.85" 0>
  <Vdc V1 1 280 390 18 -26 0 1 "6 V" 1>
  <C C1 1 670 360 -26 17 0 0 "100 nF" 1 "" 0 "neutral" 0>
  <C C2 1 780 430 17 -26 0 1 "100 nF" 1 "" 0 "neutral" 0>
  <GND * 1 780 500 0 0 0 0>
  <.TR TR1 1 200 620 0 67 0 0 "lin" 1 "0" 1 "5 ms" 1 "11001" 0 "Trapezoidal" 0 "2" 0 "1 ns" 0 "1e-16" 0 "150" 0 "0.001" 0 "1 pA" 0 "1 uV" 0 "26.85" 0 "1e-3" 0 "1e-6" 0 "1" 0 "CroutLU" 0 "no" 0 "yes" 0 "0" 0>
  <R R2 1 920 360 15 -26 0 1 "30 kOhm" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <R R1 1 540 430 15 -26 0 1 "30 kOhm" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
</Components>
<Wires>
  <540 320 540 360 "" 0 0 0 "">
  <570 290 780 290 "" 0 0 0 "">
  <780 290 920 290 "" 0 0 0 "">
  <280 290 510 290 "" 0 0 0 "">
  <280 290 280 360 "" 0 0 0 "">
  <280 420 280 500 "" 0 0 0 "">
  <920 290 920 330 "" 0 0 0 "">
  <920 390 920 500 "" 0 0 0 "">
  <780 290 780 360 "" 0 0 0 "">
  <700 360 780 360 "" 0 0 0 "">
  <540 360 640 360 "" 0 0 0 "">
  <780 360 780 400 "" 0 0 0 "">
  <780 460 780 500 "" 0 0 0 "">
  <540 360 540 400 "" 0 0 0 "">
  <540 460 540 500 "" 0 0 0 "">
  <920 290 920 290 "Vout" 950 260 0 "">
</Wires>
<Diagrams>
</Diagrams>
<Paintings>
</Paintings>
