#Update *.ts files

echo "------------------|Updating *.ts files|------------------"
lupdate ../*.cpp ../*.h ../ui/*.ui -ts  *.ts

echo ""
#Create the releases
echo "------------------|Updating *.qm files|------------------"
lrelease FDesktopRecorder_*.ts