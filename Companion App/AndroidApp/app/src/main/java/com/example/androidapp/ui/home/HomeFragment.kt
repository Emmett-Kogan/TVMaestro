package com.example.androidapp.ui.home

import android.os.Bundle
import android.view.Gravity
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.PopupWindow
import android.widget.TextView
import android.widget.Toast
import androidx.fragment.app.Fragment
import androidx.lifecycle.ViewModelProvider
import com.example.androidapp.R
import com.example.androidapp.databinding.FragmentHomeBinding

interface PopupOptionListener {
    fun onOptionSelected(option: String = "To begin scheduling, pair with your device.")
}

class HomeFragment : Fragment(), PopupOptionListener {

    private var _binding: FragmentHomeBinding? = null

    private lateinit var popupWindow: PopupWindow

    // This property is only valid between onCreateView and
    // onDestroyView.
    private val binding get() = _binding!!

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View {

        //val selectedOptionTextView: TextView = binding.selectedOptionTextView

        val homeViewModel =
            ViewModelProvider(this).get(HomeViewModel::class.java)

        _binding = FragmentHomeBinding.inflate(inflater, container, false)
        val root: View = binding.root

        val textView: TextView = binding.textHome
        homeViewModel.text.observe(viewLifecycleOwner) {
            textView.text = it
        }
        return root
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        // Get a reference to the button
        val pairButton: Button = binding.button1

        // Set an OnClickListener for the button
        pairButton.setOnClickListener {
            // Define the action to be performed when the button is clicked
            // For example, you can show a toast message or navigate to another fragment/activity.
            // Replace the following line with your desired action.
            // For example, you can show a toast message:
            // Toast.makeText(requireContext(), "Button Clicked", Toast.LENGTH_SHORT).show()
            showPopupWindow(pairButton)
        }
    }

    private fun showPopupWindow(anchorView: View) {
        // Inflate the popup_options.xml layout
        val popupView = layoutInflater.inflate(R.layout.pair_options, null)

        // Create the PopupWindow
        popupWindow = PopupWindow(
            popupView,
            ViewGroup.LayoutParams.WRAP_CONTENT,
            ViewGroup.LayoutParams.WRAP_CONTENT,
            true
        )

        // Set up options click listeners
        val option1 = popupView.findViewById<TextView>(R.id.option1)
        val option2 = popupView.findViewById<TextView>(R.id.option2)

        option1.setOnClickListener {
            // Handle option 1 click
            // For example, you can perform an action or dismiss the popup
            onOptionSelected("Option 1")
            popupWindow.dismiss()
        }

        option2.setOnClickListener {
            // Handle option 2 click
            // For example, you can perform an action or dismiss the popup
            onOptionSelected("Option 2")
            popupWindow.dismiss()
        }

        // Set the dismiss listener to close the popup window when it's dismissed
        popupWindow.setOnDismissListener {
            // Handle dismiss if needed
        }

        // Show the popup window
        popupWindow.showAtLocation(anchorView, Gravity.CENTER, 0, 0)
        // Set the size of the popup window after it's been shown
        val width = resources.displayMetrics.widthPixels * 0.95 // 80% of screen width
        val height = resources.displayMetrics.heightPixels * 0.6 // 60% of screen height

        popupWindow.update(width.toInt(), height.toInt())

        updateText("Device Paired.");
    }

    override fun onDestroyView() {
        super.onDestroyView()
        _binding = null
    }

    override fun onOptionSelected(option: String) {
        // Update the UI based on the selected option
        //val selectedOptionTextView: TextView = binding.selectedOptionTextView
        //selectedOptionTextView.text = "Paired Device: $option"
    }

    // Function to update text in the fragment
    fun updateText(message: String) {
        val textView: TextView = binding.text1
        textView.text = message
    }
}