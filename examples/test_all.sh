#!/bin/sh

echo "\n*Testing examples with typesafety disabled.*\n"

cd BankApp
echo "In BankApp..."
echo "-> Generating files."
ruby pico_ql_generator.rb pico_ql_dsl.sql > /dev/null
echo "-> Building."
make clean > /dev/null
make > /dev/null
echo "-> Executing tests."
./bank_app > /dev/null
cat pico_ql_test_output.txt

cd ../Chess
echo "\nIn Chess..."
echo "-> Generating files."
ruby pico_ql_generator.rb pico_ql_dsl.sql > /dev/null
echo "-> Building."
make clean > /dev/null
make > /dev/null
echo "-> Executing tests."
./chess > /dev/null
cat pico_ql_test_output.txt

cd ../VRP
echo "\nIn VRP..."
echo "-> Generating files."
ruby pico_ql_generator.rb pico_ql_dsl.sql > /dev/null
echo "-> Building."
make clean > /dev/null
make > /dev/null
echo "-> Executing tests."
./schedule cvrp/solomon.txt 2 > /dev/null
cat pico_ql_test_output.txt
echo "(Expected failure.)"

#cd ../bowtie
#echo "\nIn bowtie..."
#echo "-> Generating files."
#ruby pico_ql_generator.rb sqtl_pico_ql_dsl.sql > /dev/null
#echo "-> Building."
#make clean > /dev/null
#rm *.o /dev/null
#make > /dev/null
#echo "-> Executing tests."
#./bowtie-debug c > /dev/null
#cat pico_ql_test_output.txt
#echo "(Expected failure.)"

cd ..
echo "\n*Testing examples with typesafety enabled.*\n"

cd BankApp
echo "In BankApp..."
echo "-> Generating files."
ruby pico_ql_generator.rb pico_ql_dsl.sql typesafe > /dev/null
echo "-> Building."
make clean > /dev/null
make > /dev/null
echo "-> Executing tests."
./bank_app > /dev/null
cat pico_ql_test_output.txt

cd ../Chess
echo "\nIn Chess..."
echo "-> Generating files."
ruby pico_ql_generator.rb pico_ql_dsl.sql typesafe > /dev/null
echo "-> Building."
make clean > /dev/null
make > /dev/null
echo "-> Executing tests."
./chess > /dev/null
cat pico_ql_test_output.txt

cd ../VRP
echo "\nIn VRP..."
echo "-> Generating files."
ruby pico_ql_generator.rb pico_ql_dsl.sql typesafe > /dev/null
echo "-> Building."
make clean > /dev/null
make TYPESAFE=1 > /dev/null
echo "-> Executing tests."
./schedule cvrp/solomon.txt 2 > /dev/null
cat pico_ql_test_output.txt

echo "\nEND"