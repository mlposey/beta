pipeline {
    agent any
    stages {
        stage('Test') {
            steps {
                sh 'docker run --rm -v $(pwd):/test beta:test'
            }
        }
        stage('Create and Push Git Tag') {
            when { branch 'master' }
            steps {
                sh '/bin/bash ./ops/tag.sh'
            }
        }
        stage('Archive') {
            when { branch 'master' }
            steps {
                sh 'rm *.tar.gz || true'
                sh '/bin/bash ./ops/archive-release.sh ${BUILD_NUMBER}'
            }
            post {
                always {
                    archiveArtifacts '*.tar.gz'
                }
            }
        }
    }
}