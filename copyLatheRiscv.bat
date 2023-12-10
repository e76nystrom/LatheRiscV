set dir=
echo Directory %dir%
set base=neorv32\sw\example\LatheRiscV
set files=*.*
set src=C:\Development\%base%
set dst=\\Shop10\Development\%base%\
echo Source %src% Destination %dst%
robocopy %src% %dst% "%files%" /E /COPY:DAT /SJ /SL /MT /NP /NS /NC /NFL /NDL /ETA
