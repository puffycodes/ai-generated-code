# Spring Boot Web Server

A minimal web server built with Spring Boot that implements the specified features.

## Features

- Home page with welcome message and navigation
- Message submission form with email and message fields
- Result page displaying submitted data
- CSS styling for all pages
- Navigation links between pages

## Running the Application

1. Navigate to the project directory:
   ```bash
   cd ../ai-generated-code/src/web-server/spring-boot/
   ```

2. Run the application using Maven:
   ```bash
   ./mvnw spring-boot:run
   ```
   
   Or if you have Maven installed:
   ```bash
   mvn spring-boot:run
   ```

3. Open your browser and go to: http://localhost:8080

## Project Structure

- `src/main/java/` - Java source code
- `src/main/resources/templates/` - Thymeleaf HTML templates
- `src/main/resources/static/css/` - CSS stylesheets
- `pom.xml` - Maven configuration
