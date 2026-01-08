package com.example.securewebserver.controller;

import com.example.securewebserver.model.Message;
import com.example.securewebserver.repository.MessageRepository;
import jakarta.validation.Valid;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.validation.BindingResult;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.servlet.mvc.support.RedirectAttributes;

@Controller
public class WebController {
    private static final Logger logger = LoggerFactory.getLogger(WebController.class);
    
    @Autowired
    private MessageRepository messageRepository;
    
    @GetMapping("/")
    public String home() {
        return "home";
    }
    
    @GetMapping("/submit")
    public String submitForm(Model model) {
        model.addAttribute("message", new Message());
        return "submit";
    }
    
    @PostMapping("/submit")
    public String submitMessage(@Valid Message message, BindingResult result, 
                               RedirectAttributes redirectAttributes) {
        if (result.hasErrors()) {
            logger.warn("Form validation failed for email: {}", message.getEmail());
            return "submit";
        }
        
        try {
            Message savedMessage = messageRepository.save(message);
            logger.info("Message saved successfully with ID: {}", savedMessage.getId());
            redirectAttributes.addAttribute("id", savedMessage.getId());
            return "redirect:/confirmation";
        } catch (Exception e) {
            logger.error("Error saving message", e);
            redirectAttributes.addFlashAttribute("error", "An error occurred. Please try again.");
            return "redirect:/submit";
        }
    }
    
    @GetMapping("/confirmation")
    public String confirmation(Long id, Model model, RedirectAttributes redirectAttributes) {
        if (id == null) {
            return "redirect:/";
        }
        
        try {
            Message message = messageRepository.findById(id).orElse(null);
            if (message == null) {
                logger.warn("Message not found for ID: {}", id);
                redirectAttributes.addFlashAttribute("error", "Message not found.");
                return "redirect:/";
            }
            model.addAttribute("message", message);
            return "confirmation";
        } catch (Exception e) {
            logger.error("Error retrieving message with ID: {}", id, e);
            redirectAttributes.addFlashAttribute("error", "An error occurred.");
            return "redirect:/";
        }
    }
}
