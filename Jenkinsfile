pipeline {
  agent any
  stages {
    stage('Build') {
      steps {
        echo 'Building'
      }
    }

    stage('Test') {
      parallel {
        stage('Test') {
          steps {
            echo 'Testing'
          }
        }

        stage('Integration Tests') {
          steps {
            echo 'Integration Testing'
          }
        }

        stage('Performance Testing') {
          steps {
            timeout(time: 90) {
              echo 'Performance testing complete'
            }

          }
        }

      }
    }

    stage('Deploy') {
      steps {
        echo 'Deploying'
      }
    }

  }
}