package com.example.webserver.controller;

import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestParam;

@Controller
public class WebController {

    @GetMapping("/")
    public String home() {
        return "home";
    }

    @GetMapping("/submit")
    public String submitPage() {
        return "submit";
    }

    @PostMapping("/submit")
    public String handleSubmit(@RequestParam String email, @RequestParam String message, Model model) {
        model.addAttribute("email", email);
        model.addAttribute("message", message);
        return "result";
    }
}
