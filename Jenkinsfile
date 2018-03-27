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
                sh '/bin/bash ops/tag.sh src/version.h'
            }
        }
        stage('Build and Push Docker Image') {
            when { branch 'master' }
            environment {
                DOCKER_USER = credentials('DOCKER_USER')                
                DOCKER_PASSWORD = credentials('DOCKER_PASSWORD')
            }
            steps {
                sh '/bin/bash ops/build-image.sh src/version.h'
            }
        }
        stage('Archive') {
            when { branch 'master' }
            steps {
                sh 'rm *.tar.gz || true'
                sh '/bin/bash ops/archive-release.sh src/version.h'
            }
            post {
                always {
                    archiveArtifacts '*.tar.gz'
                }
            }
        }
    }
}