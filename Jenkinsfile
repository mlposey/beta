pipeline {
    agent any
    stages {
        stage('Test') {
            steps {
                sh 'docker run --rm -v $(pwd):/test beta:test'
            }
        }
        stage('Archive') {
            when { branch 'master' }
            steps {
                sh 'sh archive-release.sh ${BUILD_NUMBER}'
            }
        }
    }
}