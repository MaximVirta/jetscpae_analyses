seq 0 99 | xargs -n1 -I{} -P8 root.exe -q -l -b 'jetscapeToJTree.C("trees/jetscape_jTree_{}.root", "output/output_{}_final_state_hadrons.dat", "{}")'
