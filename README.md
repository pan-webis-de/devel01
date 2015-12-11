# devel01
de Vel, O., Anderson, A., Corney, M., &amp; Mohay, G. (2001). Mining e-mail content for author identification forensics. SIGMOD Record, 30(4), 55-64.

#Implementation
Create FeatureExtracter.exe and JsonWriter.exe using the sourcecode in the corresponding folders.
Use svm-train.exe and svm-predict.exe in order to run the classification (https://www.csie.ntu.edu.tw/~cjlin/libsvm/).

# Example
1. Run FeatureExtracter(creates training and test files):
./FeatureExtracter.exe "./NEW CORPORA/pan11small" "train.dat" "test.dat"

2. Run svm-train.exe (creates a model file):
svm-train.exe -b 1 -c 20 -t 1 -d 3 train.dat model

3. Run svm-predict.exe (creates a prediction file):
svm-predict.exe -b 1 test.dat model predict

4. Run JsonWriter (creates a json file):
./JsonWriter.exe "predict" "devel01.json"
