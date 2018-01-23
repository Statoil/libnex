#!/usr/bin/env groovy

pipeline {
    agent none

    stages {
        stage('build') {
            agent { label 'si-build' }
            steps {
                sh """
                    rm -rf libecl

                    PYTHON_VERSION=2.7.13 GCC_VERSION="4.9.4" source /prog/sdpsoft/env.sh
                    root_dir=\$PWD

                    virtualenv venv
                    source venv/bin/activate

                    pip install --upgrade -r requirements.txt

                    git clone https://github.com/Statoil/libecl.git
                    mkdir libecl/build
                    pushd libecl/build
                    pip install --upgrade -r ../requirements.txt
                    /project/res/x86_64_RH_6/bin/cmake \
                          -DBUILD_SHARED_LIBS=ON \
                          -DENABLE_PYTHON=ON \
                          -DCMAKE_CXX_COMPILER=/opt/rh/devtoolset-3/root/usr/bin/g++ \
                          -DCMAKE_INSTALL_PREFIX=\$root_dir/ecl_install \
                          ..
                    make install -j3
                    popd
                    export LD_LIBRARY_PATH="\$root_dir/ecl_install/lib64:\$LD_LIBRARY_PATH"

                    mkdir nex_install
                    mkdir build
                    pushd build
                    /project/res/x86_64_RH_6/bin/cmake \
                            -Decl_DIR=\$root_dir/ecl_install/share/cmake/ecl \
                            -DCMAKE_CXX_COMPILER=/opt/rh/devtoolset-3/root/usr/bin/g++ \
                            ..
                    make
                    popd
                    deactivate
                   """
                stash name: 'nex-build', includes: 'build/'
                stash name: 'ecl', includes: 'ecl_install/'
                stash name: 'venv', includes: 'venv/'
            }
            post {
                always {
                    deleteDir()
                }
            }
        }
        stage('test'){
            agent { label 'si-build' }
            steps {
                unstash 'ecl'
                unstash 'nex-build'
                unstash 'venv'
                sh """
                    PYTHON_VERSION=2.7.13 GCC_VERSION="4.9.4" source /prog/sdpsoft/env.sh
                    root_dir=\$PWD
                    export LD_LIBRARY_PATH="\$root_dir/ecl_install/lib:\$LD_LIBRARY_PATH"
                    export PYTHONPATH="\$root_dir/ecl_install/lib/python2.7/site-packages/:\$PYTHONPATH"
                    source venv/bin/activate
                    python -c "import ecl; print ecl.__file__"
                    pushd build
                    make CTEST_OUTPUT_ON_FAILURE=1 test
                    deactivate
                   """

            }
            post {
                always {
                    deleteDir()
                }
            }
        }
    }
}
