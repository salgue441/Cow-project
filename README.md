# Cow classification project

## Introduction

## Pre-requisites

- Docker
- Docker compose
- Python 3.11
- Pyspark

## Getting started

1. Clone the repository

```bash
git clone git@github.com:salgue441/cow-project.git
cd cow-project
```

2. Build the docker image

```bash
docker-compose build
```

3. Start the spark cluster and ML application

```bash
docker-compose up
```

## Usage

The ML application will automatically start training when you run `docker-compose up`. You can modify the entry point in the `docker-compose.yml` file to run different scripts or Jupyter notebooks. To access the Spark UI, navigate to `http://localhost:8080` in your web browser.

## Development

1. Make changes to the code in the `src/` directory.
2. Update tests in the `tests/` directory as needed.
3. Run tests locally:

```bash
docker-compose run ml-application python -m unittest discover tests
```

4. Commit your changes and push to GitHub. The CI/CD pipeline will run tests and linting automatically.

## Contributing

Please read [CONTRIBUTING.md](CONTRIBUTING.md) for details on our code of conduct and the process for submitting pull requests.

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details.
