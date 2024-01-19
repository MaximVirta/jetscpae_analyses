export ALIBUILD_WORK_DIR=/home/lib/alice/sw
cd /home/host

alienv setenv AliPhysics/latest-master-o2 -c aliroot -q -l 'JCorrAnalysis.C("inputTree.txt", "output_test.root")'
