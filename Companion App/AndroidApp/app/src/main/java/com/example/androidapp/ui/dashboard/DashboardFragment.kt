package com.example.androidapp.ui.dashboard

import android.os.Bundle
import android.view.Gravity
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.PopupWindow
import android.widget.TextView
import androidx.fragment.app.Fragment
import androidx.lifecycle.ViewModelProvider
import com.example.androidapp.R
import com.example.androidapp.databinding.FragmentDashboardBinding

class DashboardFragment : Fragment() {

    private var _binding: FragmentDashboardBinding? = null

    // This property is only valid between onCreateView and
    // onDestroyView.
    private val binding get() = _binding!!

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View {
        val dashboardViewModel =
            ViewModelProvider(this).get(DashboardViewModel::class.java)

        _binding = FragmentDashboardBinding.inflate(inflater, container, false)
        val root: View = binding.root

        val textView: TextView = binding.textDashboard
        dashboardViewModel.text.observe(viewLifecycleOwner) {
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
        val popupView = layoutInflater.inflate(R.layout.add_schedules, null)

        // Create the PopupWindow
        var popupWindow = PopupWindow(
            popupView,
            ViewGroup.LayoutParams.WRAP_CONTENT,
            ViewGroup.LayoutParams.WRAP_CONTENT,
            true
        )

        // Set up options click listeners
        val option1 = popupView.findViewById<TextView>(R.id.option1)
        val option2 = popupView.findViewById<TextView>(R.id.option2)
        val option3 = popupView.findViewById<TextView>(R.id.option3)

        option1.setOnClickListener {
            // Handle option 1 click
            // For example, you can perform an action or dismiss the popup

            popupWindow.dismiss()
        }

        option2.setOnClickListener {
            // Handle option 2 click
            // For example, you can perform an action or dismiss the popup

            popupWindow.dismiss()
        }

        option3.setOnClickListener {
            // Handle option 2 click
            // For example, you can perform an action or dismiss the popup

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

        updateText("Channel A Selected.");
    }

    override fun onDestroyView() {
        super.onDestroyView()
        _binding = null
    }

    // Function to update text in the fragment
    fun updateText(message: String) {
        val textView: TextView = binding.textView1
        textView.text = message
    }
}

