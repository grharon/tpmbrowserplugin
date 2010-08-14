echo "build xpi"
mkdir xpi
mkdir xpi\plugins
copy /y ..\debug\nprt2.dll xpi\plugins\nprt.dll
copy /y .\install.rdf xpi\
cd xpi
jar cvfM ..\nprt.xpi -C ./ *.*
cd ..

rmdir /q /s xpi
echo "Open nprt.xpi with firefox. The plugin will be installed into :"
echo "<home>\Application Data\Mozilla\Firefox\Profiles\<profile_name>\extensions\"
