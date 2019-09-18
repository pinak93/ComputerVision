gcc -o lab1 lab1.c
gcc -o 1D 1D.c
gcc -o slide slide.c
gcc -o diff diff.c
output1=$(./lab1)
output2=$(./lab1)
output3=$(./lab1)
output4=$(./lab1)
output5=$(./lab1)
output6=$(./lab1)
output7=$(./lab1)
output8=$(./lab1)
output9=$(./lab1)
output10=$(./lab1)

lab1=$(((output1+output2+output3+output4+output5+output6+output7+output8+output9+output10)/10))
echo  Average of 2D Filter- $lab1
output1=$(./1D)
output2=$(./1D)
output3=$(./1D)
output4=$(./1D)
output5=$(./1D)
output6=$(./1D)
output7=$(./1D)
output8=$(./1D)
output9=$(./1D)
output10=$(./1D)


D1=$(((output1+output2+output3+output4+output5+output6+output7+output8+output9+output10)/10))
echo  Average of Seprate Filter 1D - $D1


output1=$(./slide)
output2=$(./slide)
output3=$(./slide)
output4=$(./slide)
output5=$(./slide)
output6=$(./slide)
output7=$(./slide)
output8=$(./slide)
output9=$(./slide)
output10=$(./slide)


slide=$(((output1+output2+output3+output4+output5+output6+output7+output8+output9+output10)/10))
echo  Average of Sliding Window - $slide

echo 2D Filter and Seperate Filter $(./diff 2DFilter.ppm SeptFilter.ppm)
echo 2D Filter and SlidingWindow $(./diff 2DFilter.ppm Sliding.ppm)
echo SlidingWindow and Seperate Filter $(./diff Sliding.ppm SeptFilter.ppm)

if [ $lab1 > $D1 ];
then
	if [ $slide > $D1 ]; then
   		echo "Sliding Filter is Fastest"
	else
		echo "Seprate Filter is Fastest"
	fi
else
	if [$lab1<$slide]; then
		echo "1D is Fastest"
	else
		echo "Sliding Filter is Fastest"
	fi

fi






